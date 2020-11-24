.include <bsd.own.mk>

PREFIX?=	/usr/local
LIBDIR=		${PREFIX}/lib/pkg
SHLIB_DIR?=	${LIBDIR}/
SHLIB_NAME?=	diff.so

SRCS=		plugin.c pkgdiff.c

PKGFLAGS!=	pkgconf --cflags pkg
CFLAGS+=	${PKGFLAGS}
LDFLAGS+=	-L/usr/local/lib -Wl,-Bstatic -lmba -Wl,-Bdynamic

beforeinstall:
	${INSTALL} -d ${LIBDIR}

.include <bsd.lib.mk>
