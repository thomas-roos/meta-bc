DESCRIPTION = "Audio/video SIP-based IP phone (console edition)"
LICENSE = "AGPL-3.0-only"
HOMEPAGE = "http://www.linphone.org/technical-corner/linphone/overview"
SECTION = "network"
SECTION_linphonec =  "console/network"

INC_PR = "r0"

DEPENDS = "libbellesip libortp libmediastreamer2 libxml2 libopus libbctoolbox belr belcard openssl xerces-c soci jsoncpp pkgconfig-native"
RDEPENDS:${PN} = "libopus"

PROVIDES = "liblinphone linphonec"

# Prevent GCC 15.2 from using absolute paths for system libraries
LDFLAGS += "-Wl,--no-as-needed -Wl,-rpath-link,${STAGING_LIBDIR}"
TARGET_LDFLAGS += "-L${STAGING_LIBDIR}"

inherit cmake gettext python3native

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
    sed -i '/^project(/a find_package(OpenSSL REQUIRED)' ${S}/CMakeLists.txt
    sed -i 's/find_package(ORTP REQUIRED)/find_package(PkgConfig REQUIRED)\npkg_check_modules(ORTP REQUIRED ortp)/' ${S}/CMakeLists.txt
    sed -i 's/find_package(BcToolbox [0-9.]* REQUIRED.*/pkg_check_modules(BCTOOLBOX REQUIRED bctoolbox)/' ${S}/CMakeLists.txt
    sed -i 's/calloc(sizeof(type),n)/calloc(n,sizeof(type))/' ${S}/coreapi/lpconfig.c
}

python do_patch:append() {
    s = d.getVar('S')
    f = f'{s}/src/db/main-db.cpp'
    with open(f, 'r') as file:
        content = file.read()
    
    # Fix 1: before + 1 and after + 1
    content = content.replace(
        'soci::rowset<soci::row> rows = (d->dbSession.getBackendSession()->prepare << query, soci::use(dbChatRoomId),\n\t\t\t                                soci::use(dbEventId), soci::use(before + 1), soci::use(after + 1));',
        'unsigned int beforePlusOne = before + 1;\n\t\t\tunsigned int afterPlusOne = after + 1;\n\t\t\tsoci::rowset<soci::row> rows = (d->dbSession.getBackendSession()->prepare << query, soci::use(dbChatRoomId),\n\t\t\t                                soci::use(dbEventId), soci::use(beforePlusOne), soci::use(afterPlusOne));'
    )
    
    # Fix 2: chatMessage->getStorageId() at line ~1515
    content = content.replace(
        '\t\tsoci::rowset<soci::row> rows =\n\t\t    (session->prepare << query, soci::use(chatMessage->getStorageId()), soci::use(meAddressId));',
        '\t\tlong long storageId = chatMessage->getStorageId();\n\t\tsoci::rowset<soci::row> rows =\n\t\t    (session->prepare << query, soci::use(storageId), soci::use(meAddressId));'
    )
    
    # Fix 3: chatMessage->getStorageId() at line ~1538
    content = content.replace(
        '\t\tsoci::rowset<soci::row> rows = (session->prepare << query, soci::use(chatMessage->getStorageId()));',
        '\t\tlong long storageId2 = chatMessage->getStorageId();\n\t\tsoci::rowset<soci::row> rows = (session->prepare << query, soci::use(storageId2));'
    )
    
    # Fix 6: soci::use(0), soci::use(1) at line ~1914
    content = content.replace(
        '\t\tconst long long &organizerId = dbSession.resolveId(organizerRow, 3);\n\t\t*session << "INSERT INTO conference_info_participant (conference_info_id, participant_sip_address_id, deleted, "\n\t\t            "is_organizer) VALUES (:conferenceInfoId, :participantSipAddressId, :deleted, :isOrganizer)",\n\t\t    soci::use(dbConferenceInfoId), soci::use(organizerSipAddressId), soci::use(0), soci::use(1);',
        '\t\tconst long long &organizerId = dbSession.resolveId(organizerRow, 3);\n\t\tint deleted = 0;\n\t\tint isOrganizer = 1;\n\t\t*session << "INSERT INTO conference_info_participant (conference_info_id, participant_sip_address_id, deleted, "\n\t\t            "is_organizer) VALUES (:conferenceInfoId, :participantSipAddressId, :deleted, :isOrganizer)",\n\t\t    soci::use(dbConferenceInfoId), soci::use(organizerSipAddressId), soci::use(deleted), soci::use(isOrganizer);'
    )
    
    # Fix 7: soci::use(std::string()) at line ~1975
    content = content.replace(
        '\t\t\t\t// Set parameter string to an empty string\n\t\t\t\t*session << "UPDATE conference_info_participant SET params = :paramsStr  WHERE conference_info_id  = "\n\t\t\t\t            ":conferenceInfoId",\n\t\t\t\t    soci::use(std::string()), soci::use(dbConferenceInfoId);',
        '\t\t\t\t// Set parameter string to an empty string\n\t\t\t\tstd::string emptyStr;\n\t\t\t\t*session << "UPDATE conference_info_participant SET params = :paramsStr  WHERE conference_info_id  = "\n\t\t\t\t            ":conferenceInfoId",\n\t\t\t\t    soci::use(emptyStr), soci::use(dbConferenceInfoId);'
    )
    
    # Fix 4: dbSession.getLastInsertId() at line ~2733 - place BEFORE the *session statement
    content = content.replace(
        '\t\tif (!isNull)\n\t\t\t\t*session << \"INSERT INTO friend_app_data (friend_id, name, data) VALUES\"\n\t\t\t\t            \" (:friendId, \'legacy\', :data)\",\n\t\t\t\t    soci::use(dbSession.getLastInsertId()), soci::use(data);',
        '\t\tif (!isNull) {\n\t\t\t\tlong long lastInsertId = dbSession.getLastInsertId();\n\t\t\t\t*session << \"INSERT INTO friend_app_data (friend_id, name, data) VALUES\"\n\t\t\t\t            \" (:friendId, \'legacy\', :data)\",\n\t\t\t\t    soci::use(lastInsertId), soci::use(data);\n\t\t\t}'
    )
    
    # Fix 5: EPHEMERAL_MESSAGE_TASKS_MAX_NB - place BEFORE the ternary operator
    content = content.replace(
        '\tauto epoch = d->dbSession.getTimeWithSociIndicator(0);\n\t\tsoci::rowset<soci::row> rows =\n\t\t    getBackend() == MainDb::Backend::Sqlite3\n\t\t        ? (d->dbSession.getBackendSession()->prepare << query, soci::use(epoch.first),\n\t\t           soci::use(EPHEMERAL_MESSAGE_TASKS_MAX_NB))',
        '\tauto epoch = d->dbSession.getTimeWithSociIndicator(0);\n\t\tint maxNb = EPHEMERAL_MESSAGE_TASKS_MAX_NB;\n\t\tsoci::rowset<soci::row> rows =\n\t\t    getBackend() == MainDb::Backend::Sqlite3\n\t\t        ? (d->dbSession.getBackendSession()->prepare << query, soci::use(epoch.first),\n\t\t           soci::use(maxNb))'
    )
    
    # Fix 8: Remove ms_crypto_suite_to_string debug logging (function doesn't exist in this mediastreamer2 version)
    content = content.replace(
        '\t\t} else if (isEncryptionMandatory() && ms_crypto_suite_is_unencrypted(suite)) {\n\t\t\tlWarning() << "Not offering " << std::string(ms_crypto_suite_to_string(suite))\n\t\t\t           << " because either RTP or RTCP streams is not encrypted";\n\t\t} else {\n\t\t\tlWarning() << "Not offering " << std::string(ms_crypto_suite_to_string(suite));\n\t\t}',
        '\t\t} else if (isEncryptionMandatory() && ms_crypto_suite_is_unencrypted(suite)) {\n\t\t\tlWarning() << "Not offering crypto suite because either RTP or RTCP streams is not encrypted";\n\t\t} else {\n\t\t\tlWarning() << "Not offering crypto suite";\n\t\t}'
    )
    
    # Fix 8: Remove ms_crypto_suite_to_string debug logging in media-session.cpp
    f2 = f'{s}/src/conference/session/media-session.cpp'
    with open(f2, 'r') as file:
        content2 = file.read()
    
    content2 = content2.replace(
        '\t\t} else if (isEncryptionMandatory() && ms_crypto_suite_is_unencrypted(suite)) {\n\t\t\tlWarning() << "Not offering " << std::string(ms_crypto_suite_to_string(suite))\n\t\t\t           << " because either RTP or RTCP streams is not encrypted";\n\t\t} else {\n\t\t\tlWarning() << "Not offering " << std::string(ms_crypto_suite_to_string(suite));\n\t\t}',
        '\t\t} else if (isEncryptionMandatory() && ms_crypto_suite_is_unencrypted(suite)) {\n\t\t\tlWarning() << "Not offering crypto suite because either RTP or RTCP streams is not encrypted";\n\t\t} else {\n\t\t\tlWarning() << "Not offering crypto suite";\n\t\t}'
    )
    
    with open(f2, 'w') as file:
        file.write(content2)
    
    with open(f, 'w') as file:
        file.write(content)
}

PACKAGECONFIG ??= "sqlite zlib video"
PACKAGECONFIG[sqlite] = "-DENABLE_SQLITE_STORAGE=yes, -DENABLE_SQLITE_STORAGE=no, sqlite3"
PACKAGECONFIG[zlib] = ", --disable-zlib, zlib"
PACKAGECONFIG[video] = ",-DENABLE_VIDEO=NO"
PACKAGECONFIG[mdns] = "-DENABLE_MDNS=YES,-DENABLE_MDNS=NO"

EXTRA_OECMAKE += " -DENABLE_CONSOLE_UI=YES -DENABLE_GTK_UI=NO -DCMAKE_SKIP_INSTALL_RPATH=ON -DENABLE_CXX_WRAPPER=NO -DENABLE_VCARD=NO -DENABLE_STRICT=NO -DENABLE_LIME_X3DH=NO -DENABLE_QRCODE=NO -DENABLE_UNIT_TESTS=NO -DENABLE_TOOLS=NO -DENABLE_DAEMON=NO"
EXTRA_OECMAKE += "-DCMAKE_INSTALL_DATAROOTDIR=share -DCMAKE_INSTALL_LIBDIR=lib"

# Fix cross-compilation include directories cmake problem
# Check Yocto version with DISTRO_VERSION variable to add or not the two following lines
# Removed obsolete base_version_less_or_equal checks for modern Yocto

INSANE_SKIP:${PN} += "dev-deps"
ERROR_QA:remove = "patch-fuzz"

PACKAGES += " ${PN}c ${PN}-common ${PN}-tester ${PN}-utils ${PN}-rings"

FILES:${PN} += "${bindir}/linphone-daemon \
		${bindir}/linphone-daemon-pipetest "
#		${libdir}/liblinphone.so.9.0.0 \
#		${libdir}/liblinphone.so.9 "

FILES:${PN}-common = "${bindir}/lp-gen-wrappers \
			${datadir}/pixmaps \
			${datadir}/applications \
			${datadir}/gnome \
			${datadir}/tutorials \
			${datadir}/linphone \
			${datadir}/sounds/linphone/hello8000.wav \
			${datadir}/sounds/linphone/hello16000.wav \
			${datadir}/sounds/linphone/incoming_chat.wav \
			${datadir}/sounds/linphone/ringback.wav \
			${datadir}/images/nowebcamCIF.jpg \
			${datadir}/appdata/linphone.appdata.xml \
			${datadir}/icons"

FILES:${PN}-utils = "${bindir}/lp-test-ecc \
			${bindir}/lp-auto-answer \
			${bindir}/xml2lpc_test \
			${bindir}/lpc2xml_test \
			${bindir}/lp-sendmsg \"

FILES:${PN}-tester = "${bindir}/liblinphone_tester \
			${datadir}/liblinphone_tester/messages.db \
			${datadir}/liblinphone_tester/tester_hosts \
			${datadir}/liblinphone_tester/certificates \
			${datadir}/liblinphone_tester/images \
			${datadir}/liblinphone_tester/common \
			${datadir}/liblinphone_tester/rcfiles \
			${datadir}/liblinphone_tester/sounds \
			${datadir}/liblinphone_tester/sipp \
			${datadir}/liblinphone_tester/certificates/cn \
			${datadir}/liblinphone_tester/certificates/altname \
			${datadir}/liblinphone_tester/certificates/cn/openssl-cn.cnf \
			${datadir}/liblinphone_tester/certificates/cn/cafile.pem \
			${datadir}/liblinphone_tester/certificates/cn/agent.pem \
			${datadir}/liblinphone_tester/certificates/altname/cafile.pem \
			${datadir}/liblinphone_tester/certificates/altname/openssl-altname.cnf \
			${datadir}/liblinphone_tester/certificates/altname/agent.pem \
			${datadir}/liblinphone_tester/images/nowebcamCIF.jpg \
			${datadir}/liblinphone_tester/common/bc_completion \
			${datadir}/liblinphone_tester/rcfiles/marie_remote_invalid_uri_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_rc \
			${datadir}/liblinphone_tester/rcfiles/pauline_alt_rc \
			${datadir}/liblinphone_tester/rcfiles/pauline_zrtp_srtpsuite_aes256_rc \
			${datadir}/liblinphone_tester/rcfiles/multi_account_rc \
			${datadir}/liblinphone_tester/rcfiles/michelle_rc \
			${datadir}/liblinphone_tester/rcfiles/pauline_rc_rtcp_xr \
			${datadir}/liblinphone_tester/rcfiles/marie_remote_https_rc \
			${datadir}/liblinphone_tester/rcfiles/zero_length_params_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_remote_404_rc \
			${datadir}/liblinphone_tester/rcfiles/stun_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_zrtp_srtpsuite_aes256_rc \
			${datadir}/liblinphone_tester/rcfiles/pauline_sips_rc \
			${datadir}/liblinphone_tester/rcfiles/upnp_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_remote_invalid_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_zrtp_b256_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_sips_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_remote_localfile_rc \
			${datadir}/liblinphone_tester/rcfiles/pauline_h264_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_remote_localfile2_rc \
			${datadir}/liblinphone_tester/rcfiles/laure_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_remote_localfile_win10_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_remote_default_values_rc \
			${datadir}/liblinphone_tester/rcfiles/remote_zero_length_params_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_remote_rc \
			${datadir}/liblinphone_tester/rcfiles/pauline_wild_rc \
			${datadir}/liblinphone_tester/rcfiles/pauline_zrtp_b256_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_transient_remote_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_early_rc \
			${datadir}/liblinphone_tester/rcfiles/empty_rc \
			${datadir}/liblinphone_tester/rcfiles/pauline_tcp_rc \
			${datadir}/liblinphone_tester/rcfiles/pauline_zrtp_aes256_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_remote_localfile_android_rc \
			${datadir}/liblinphone_tester/rcfiles/laure_call_logs_rc \
			${datadir}/liblinphone_tester/rcfiles/pauline_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_rc_rtcp_xr \
			${datadir}/liblinphone_tester/rcfiles/marie_quality_reporting_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_h264_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_zrtp_aes256_rc \
			${datadir}/liblinphone_tester/sounds/ringback.wav \
			${datadir}/liblinphone_tester/sounds/sintel_trailer_opus_h264.mkv \
			${datadir}/liblinphone_tester/sounds/hello8000.wav \
			${datadir}/liblinphone_tester/sounds/vrroom.wav \
			${datadir}/liblinphone_tester/sounds/sintel_trailer_pcmu_h264.mkv \
			${datadir}/liblinphone_tester/sounds/hello8000.mkv \
			${datadir}/liblinphone_tester/sounds/oldphone.wav \
			${datadir}/liblinphone_tester/sounds/hello8000_mkv_ref.wav \
			${datadir}/liblinphone_tester/sounds/ahbahouaismaisbon.wav \
			${datadir}/liblinphone_tester/sounds/sintel_trailer_opus_vp8.mkv \
			${datadir}/liblinphone_tester/sipp/call_with_audio_mline_before_video_in_sdp.xml \
			${datadir}/liblinphone_tester/sipp/call_with_multiple_video_mline_in_sdp.xml \
			${datadir}/liblinphone_tester/sipp/call_with_video_mline_before_audio_in_sdp.xml \
			${datadir}/liblinphone_tester/sipp/sip_update_within_icoming_reinvite_with_no_sdp.xml \
			${datadir}/liblinphone_tester/sipp/call_with_multiple_audio_mline_in_sdp.xml \
			${datadir}/liblinphone_tester/sipp/call_invite_200ok_without_contact_header.xml \
			${datadir}/liblinphone_tester/local_tester_hosts \
			${datadir}/liblinphone_tester/vcards \
			${datadir}/liblinphone_tester/vcards/thousand_vcards.vcf \
			${datadir}/liblinphone_tester/vcards/vcards.vcf \
			${datadir}/liblinphone_tester/rcfiles/marie_zrtp_ecdh255_rc \
			${datadir}/liblinphone_tester/rcfiles/marie_zrtp_ecdh448_rc \
			${datadir}/liblinphone_tester/rcfiles/pauline_zrtp_ecdh225_rc \
			${datadir}/liblinphone_tester/rcfiles/pauline_zrtp_ecdh448_rc"

FILES:${PN}c = "${bindir}/linphonec \
		${bindir}/linphonecsh \
		${datadir}/sounds/linphone/ringback.wav \
		${datadir}/sounds/linphone/rings/oldphone-mono.wav"

FILES:${PN}-rings = "${datadir}/sounds/linphone/rings"

FILES:${PN}-dev += " \
    ${includedir} \
    ${libdir}/pkgconfig \
    ${libdir}/liblinphone \
    ${datadir}/LibLinphone \
    ${datadir}/belr \
    ${prefix}/share/Linphone "
