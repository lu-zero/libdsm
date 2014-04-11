ACLOCAL_AMFLAGS = -I m4

DISTCLEANFILES = @DOLT_CLEANFILES@

EXTRA_DIST =

CFLAGS = -Icontrib -Iinclude

bdsmdir = $(includedir)/bdsm

bdsm_HEADERS = \
    include/bdsm.h			\
    include/bdsm/context.h		\
    include/bdsm/netbios_defs.h		\
    include/bdsm/netbios_query.h	\
    include/bdsm/netbios_utils.h	\
    include/bdsm/smb_file.h		\
    include/bdsm/smb_ntlm.h		\
    include/bdsm/smb_session.h		\
    include/bdsm/smb_trans2.h		\
    include/bdsm/hmac_md5.h		\
    include/bdsm/netbios_ns.h		\
    include/bdsm/netbios_session.h	\
    include/bdsm/smb_defs.h		\
    include/bdsm/smb_message.h		\
    include/bdsm/smb_packets.h		\
    include/bdsm/smb_share.h		\
    include/bdsm/smb_utils.h

lib_LTLIBRARIES = libdsm.la

libdsm_la_SOURCES = \
    contrib/mdx/md5.c		\
    contrib/mdx/md4.c		\
    src/hmac_md5.c		\
    src/netbios_utils.c		\
    src/netbios_ns.c		\
    src/netbios_ns_entry.c	\
    src/netbios_query.c		\
    src/netbios_session.c	\
    src/smb_utils.c		\
    src/smb_message.c		\
    src/smb_session.c		\
    src/smb_ntlm.c		\
    src/smb_share.c		\
    src/smb_file.c		\
    src/smb_trans2.c		\
    src/context.c

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libdsm.pc

libdsm_la_LDFLAGS = -version-info @BDSM_LIBTOOL_VERSION@


bin_PROGRAMS =

if PROGRAMS
bin_PROGRAMS += dsm dsm_discover dsm_inverse dsm_lookup
endif

dsm_SOURCES = dsm.c

dsm_discover_SOURCES = discover.c

dsm_inverse_SOURCES = inverse.c

dsm_lookup_SOURCES = lookup.c

LDADD = libdsm.la

doc:
	doxygen

a : all
c : clean
re : clean all

.PHONY: c a re doc