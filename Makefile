PROG=	exifsort

NO_MAN=

EXIF_CFLAGS!=	pkg-config --cflags libexif
EXIF_LIBS!=	pkg-config --libs libexif

CFLAGS+=	-Wall -Wextra -ggdb

CFLAGS+=	${EXIF_CFLAGS}
LDFLAGS+=	${EXIF_LIBS}

.include <bsd.prog.mk>
