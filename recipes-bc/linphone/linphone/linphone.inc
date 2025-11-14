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
LDFLAGS += "-Wl,--no-as-needed"

inherit cmake gettext python3native

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
    sed -i '/^project(/a find_package(OpenSSL REQUIRED)' ${S}/CMakeLists.txt
    sed -i 's/find_package(ORTP REQUIRED)/find_package(PkgConfig REQUIRED)\npkg_check_modules(ORTP REQUIRED ortp)/' ${S}/CMakeLists.txt
    sed -i 's/find_package(BcToolbox [0-9.]* REQUIRED.*/pkg_check_modules(BCTOOLBOX REQUIRED bctoolbox)/' ${S}/CMakeLists.txt
    sed -i 's/calloc(sizeof(type),n)/calloc(n,sizeof(type))/' ${S}/coreapi/lpconfig.c
}

python do_patch:append() {
    import subprocess
    s = d.getVar('S')
    f = f'{s}/src/db/main-db.cpp'
    
    # Read file
    with open(f, 'r') as file:
        content = file.read()
    
    # Fix SOCI rvalue errors
    content = content.replace(
        'soci::rowset<soci::row> rows =\n\t\t    (session->prepare << query, soci::use(chatMessage->getStorageId()), soci::use(meAddressId));',
        'long long storageId = chatMessage->getStorageId();\n\t\tsoci::rowset<soci::row> rows =\n\t\t    (session->prepare << query, soci::use(storageId), soci::use(meAddressId));'
    )
    content = content.replace(
        'soci::rowset<soci::row> rows = (session->prepare << query, soci::use(chatMessage->getStorageId()));',
        'long long storageId2 = chatMessage->getStorageId();\n\t\tsoci::rowset<soci::row> rows = (session->prepare << query, soci::use(storageId2));'
    )
    content = content.replace(
        'soci::use(EPHEMERAL_MESSAGE_TASKS_MAX_NB))',
        'soci::use(maxNb))'
    )
    content = content.replace(
        'soci::rowset<soci::row> rows =\n\t\t    getBackend() == MainDb::Backend::Sqlite3',
        'int maxNb = EPHEMERAL_MESSAGE_TASKS_MAX_NB;\n\t\tsoci::rowset<soci::row> rows =\n\t\t    getBackend() == MainDb::Backend::Sqlite3'
    )
    content = content.replace(
        'soci::rowset<soci::row> rows = (d->dbSession.getBackendSession()->prepare << query, soci::use(dbChatRoomId),\n\t\t\t                                soci::use(dbEventId), soci::use(before + 1), soci::use(after + 1));',
        'unsigned int beforePlusOne = before + 1;\n\t\t\tunsigned int afterPlusOne = after + 1;\n\t\t\tsoci::rowset<soci::row> rows = (d->dbSession.getBackendSession()->prepare << query, soci::use(dbChatRoomId),\n\t\t\t                                soci::use(dbEventId), soci::use(beforePlusOne), soci::use(afterPlusOne));'
    )
    
    # Write file
    with open(f, 'w') as file:
        file.write(content)
}

PACKAGECONFIG ??= "sqlite zlib video"
PACKAGECONFIG[sqlite] = "-DENABLE_SQLITE_STORAGE=yes, -DENABLE_SQLITE_STORAGE=no, sqlite3"
PACKAGECONFIG[zlib] = ", --disable-zlib, zlib"
PACKAGECONFIG[video] = ",-DENABLE_VIDEO=NO"
PACKAGECONFIG[mdns] = "-DENABLE_MDNS=YES,-DENABLE_MDNS=NO"

EXTRA_OECMAKE += " -DENABLE_CONSOLE_UI=NO -DENABLE_GTK_UI=NO -DCMAKE_SKIP_INSTALL_RPATH=ON -DENABLE_CXX_WRAPPER=NO -DENABLE_VCARD=ON -DENABLE_STRICT=NO -DENABLE_LIME_X3DH=NO -DENABLE_QRCODE=NO -DENABLE_UNIT_TESTS=NO -DENABLE_TOOLS=NO -DENABLE_DAEMON=NO"
EXTRA_OECMAKE += "-DCMAKE_INSTALL_DATAROOTDIR=share -DCMAKE_INSTALL_LIBDIR=lib"

# Fix cross-compilation include directories cmake problem
# Check Yocto version with DISTRO_VERSION variable to add or not the two following lines
# Removed obsolete base_version_less_or_equal checks for modern Yocto

INSANE_SKIP:${PN} += "dev-deps"

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



