AM_CPPFLAGS = $(all_includes) -I$(top_srcdir)/include -I$(OPENBSC_INCDIR) -I$(top_srcdir)/src/osmo-bts-sysmo
AM_CFLAGS = -Wall $(LIBOSMOCORE_CFLAGS) $(LIBOSMOGSM_CFLAGS) $(LIBOSMOVTY_CFLAGS) $(LIBOSMOTRAU_CFLAGS) $(ORTP_CFLAGS)
LDADD = $(LIBOSMOCORE_LIBS) $(LIBOSMOGSM_LIBS) $(LIBOSMOVTY_LIBS) $(LIBOSMOTRAU_LIBS) $(ORTP_LIBS)

noinst_PROGRAMS = sysmobts_test
EXTRA_DIST = sysmobts_test.ok

sysmobts_test_SOURCES = sysmobts_test.c $(top_srcdir)/src/osmo-bts-sysmo/utils.c \
		$(top_srcdir)/src/osmo-bts-sysmo/l1_if.c \
		$(top_srcdir)/src/osmo-bts-sysmo/oml.c \
		$(top_srcdir)/src/osmo-bts-sysmo/l1_transp_hw.c \
		$(top_srcdir)/src/osmo-bts-sysmo/tch.c \
		$(top_srcdir)/src/osmo-bts-sysmo/calib_file.c \
		$(top_srcdir)/src/osmo-bts-sysmo/calib_fixup.c \
		$(top_srcdir)/src/osmo-bts-sysmo/misc/sysmobts_par.c \
		$(top_srcdir)/src/osmo-bts-sysmo/eeprom.c
sysmobts_test_LDADD = $(top_builddir)/src/common/libbts.a $(LIBOSMOABIS_LIBS) $(LDADD)
