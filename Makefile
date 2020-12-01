.include <bsd.own.mk>

PREFIX?=	/usr/local
LIBDIR=		${PREFIX}/lib/pkg
SHLIB_DIR?=	${LIBDIR}/
SHLIB_NAME?=	diff.so

SRCS=		src/pkg-plugin.c src/pkgdiff.c \
		ext/libmba-0.9.1/allocator.c \
		ext/libmba-0.9.1/diff.c \
		ext/libmba-0.9.1/msgno.c \
		ext/libmba-0.9.1/suba.c \
		ext/libmba-0.9.1/varray.c

PKGFLAGS!=	pkgconf --cflags pkg
CFLAGS+=	${PKGFLAGS} -Iext/libmba-0.9.1

beforeinstall:
	${INSTALL} -d ${LIBDIR}

.include <bsd.lib.mk>
