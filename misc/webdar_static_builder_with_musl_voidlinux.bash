#!/bin/bash

if [ ! -z "$1" ] && [ "$1" -gt 1 ] ; then
    export MAKE_FLAGS="-j $1"
else
    echo "usage: $0 <num CPU cores to use>"
    exit 1
fi

# config/compilation/linking related variables

export LOCAL_PKG_CONFIG_DIR="$(pwd)/pkgconfig"

export PKG_CONFIG_PATH="${LOCAL_PKG_CONFIG_DIR}":/usr/local/lib/pkgconfig:/lib/pkgconfig:/usr/lib/pkgconfig
export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64:/usr/lib

#export LDFLAGS=-L/usr/local/lib
#export CFLAGS=-I/usr/local/include
#export CXXFLAGS=${CFLAGS}

# packages version

OPENSSL=3.3.3

# wget options need for gnutls website that does not provide all chain of trust in its certificate
GNUTLS_WGET_OPT="--no-check-certificate"
REPO=$(pwd)/REPO

#
check()
{
    # checking we are under Void Linux
    if [ $(which lsb_release | wc -l) -eq 0 ] ; then
	echo "missing lsb_release command to check sanity of the environment"
	exit 1
    fi

    if [ "$(lsb_release -i | sed -rn -e 's/^Distributor ID:\s+//p')" != "VoidLinux" ] ; then
	echo "This script is only expected to work on VoidLinux Distribution"
	exit 1
    fi

    # checking for xbps-install
    if [ "$(which xbps-install | wc -l)" -eq 0 ] ; then
	echo "Missing xbps-instal command"
	exit 1
    fi

    # checking musl libc is the system standard C library
    if [ $(xbps-query -l | grep musl | wc -l) -le 0 ] ; then
	echo "Cannot find musl standard C library"
	echo "static linking with glibc is broken, musl is needed"
	exit 1
    fi

    if [ ! -f configure -o ! -f configure.ac ] || [ $(grep DAR_VERSION configure.ac | wc -l) -ne 1 ] ; then
	echo "This script must be run from the root directory of the dar/libdar source package"
	exit 1
    fi

    if [ ! -e "${REPO}" ] ; then
	mkdir "${REPO}"
    fi

    if [ ! -d "${REPO}" ] ; then
       echo "${REPO} exists but is not a directory, aborting"
       exit 1
    fi

    if [ ! -e "${LOCAL_PKG_CONFIG_DIR}" ] ; then
	mkdir "${LOCAL_PKG_CONFIG_DIR}"
    fi

    if [ ! -d "${LOCAL_PKG_CONFIG_DIR}" ] ; then
	echo "${LOCAL_PKG_CONFIG_DIR} exists but is not a directory, aborting"
	exit 1
    fi
}

requirements()
{

    #updating xbps db
    xbps-install -SU -y

    # tools used to build the different packages involved here
    xbps-install -y gcc make wget pkg-config cmake xz || exit 1

    #direct dependencies of libdar
    xbps-install -y bzip2-devel e2fsprogs-devel libargon2-devel libgcc-devel libgcrypt-devel liblz4-devel \
		 liblzma-devel libstdc++-devel libzstd-devel lz4-devel \
		 lzo-devel musl-devel zlib-devel || exit 1

    # needed to build static flavor of librsync
    xbps-install -y libb2-devel || exit 1

    # needed to build static flavor of gnutls
    xbps-install -y  nettle-devel libtasn1-devel libunistring-devel unbound-devel unbound || exit 1

    #needed for static flavor of libcurl
    xbps-install -y libssp-devel || echo "ignoring error if libssp-devel fails to install due to musl-devel already installed"

    # need to tweak the hogweed.pc file provided by the system, we do not modify the system but shadow it by a local version located in higher priority dir
    HOGWEED_PC=/usr/lib/pkgconfig/hogweed.pc
    if [ -e "${HOGWEED_PC}" ] ; then
	sed -r -e 's/#\snettle/nettle/' < "${HOGWEED_PC}" > "${LOCAL_PKG_CONFIG_DIR}/$(basename ${HOGWEED_PC})"
    else
	echo "${HOGWEED_PC} not found"
	exit 1
    fi

    # optional but interesting to get a smaller dar_static binary
    xbps-install -y upx || echo "" && echo "WARNING!" && echo "Failed to install upx, will do without" && echo && sleep 3

    # openssl needs perl
    xbps-install -y perl || echo "" && echo "WARNING!" && echo "Failed to install perl" && exit 1

}

libssl()
{
    local LIBSSL_PKG=openssl-${LIBTSSL_VERSION}.tar.gz

    if [ ! -e "${REPO}/${LIBSSL_PKG}" ] ; then wget "https://github.com/openssl/openssl/releases/download/openssl-3.3.3/${LIBSSL_PKG}" && mv "${LIBSSL_PKG}" "${REPO}" || exit 1 ; fi
    tar -xf "${REPO}/${LIBSSL_PKG}" || exit 1
    cd libssl-${LIBTHREADAR_VERSION} || exit 1
    ./Configure && make ${MAKE_FLAGS} || exit 1
    make install
    cd ..
    ldconfig
    rm -rf openssl-${LIBTHREADAR_VERSION}
}


webdar_static()
{
    make clean || /bin/true
    make distclean || /bin/true
    ./configure --enable-static\
		--disable-shared\
		--prefix=/DAR || exit 1
    make ${MAKE_FLAGS} || exit 1
    make DESTDIR=${tmp_dir} install-strip || exit 1
    mv ${tmp_dir}/DAR/bin/webdar_static . && echo "webdar_static binary is available in the current directory"
    rm -rf ${tmp_dir}/DAR
}

check
requirements || (echo "Failed setting up requirements" && exit 1)
libssl || (echo "Failed building libssl" && exit 1)
webdar_static || (echo "Failed building dar_static" && exit 1)
