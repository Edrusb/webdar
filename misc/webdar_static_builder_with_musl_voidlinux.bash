#!/bin/bash

if [ ! -z "$1" ] && [ "$1" -gt 1 ] ; then
    export MAKE_FLAGS="-j $1"
else
    echo "usage: $0 <num CPU cores to use> [root]"
    exit 1
fi

if [ -z "$2" ] ; then
    export ROOT_PERM=no
else
    export ROOT_PERM=yes
fi

echo "ROOT_PERM = $ROOT_PERM"

# config/compilation/linking related variables

if [ "$ROOT_PERM" = "no" ] ; then
    export LOCAL_PREFIX="$HOME/usr"
else
    export LOCAL_PREFIX="/usr/local"
fi

export LOCAL_PKG_CONFIG_DIR1="$LOCAL_PREFIX/lib/pkgconfig"
export LOCAL_PKG_CONFIG_DIR2="$LOCAL_PREFIX/lib64/pkgconfig"

export PKG_CONFIG_PATH="$LOCAL_PKG_CONFIG_DIR1:$LOCAL_PKG_CONFIG_DIR2:/usr/lib/pkgconfig:/usr/lib64/pkgconfig:/usr/local/lib/pkgconfig:/usr/local/lib64/pkgconfig"
export CFLAGS="-I$LOCAL_PREFIX/include -I/usr/local/include -I/usr/include"
export CXXFLAGS="$CFLAGS"
export LDFLAGS="-L$LOCAL_PREFIX/lib -L$LOCAL_PREFIX/lib64 -L/usr/local/lib -L/usr/local/lib64 -L/usr/lib -L/usr/lib64 -L/lib -L/lib64"
export LD_LIBRARY_PATH="$LOCAL_PREFIX/lib:$LOCAL_PREFIX/lib64:/usr/local/lib:/usr/local/lib64:/usr/lib:/usr/lib64:/lib:/lib64"
export PATH="$LOCAL_PREFIX/bin:/usr/local/bin:$PATH"


# packages version

OPENSSL=3.0.16

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

    if [ ! -e "${LOCAL_PKG_CONFIG_DIR1}" ] ; then
        mkdir -p "${LOCAL_PKG_CONFIG_DIR1}"
    fi

    if [ ! -e "${LOCAL_PKG_CONFIG_DIR2}" ] ; then
        mkdir -p "${LOCAL_PKG_CONFIG_DIR2}"
    fi

    if [ ! -d "${LOCAL_PKG_CONFIG_DIR1}" ] ; then
        echo "${LOCAL_PKG_CONFIG_DIR1} exists but is not a directory, aborting"
        exit 1
    fi

    if [ ! -d "${LOCAL_PKG_CONFIG_DIR2}" ] ; then
        echo "${LOCAL_PKG_CONFIG_DIR2} exists but is not a directory, aborting"
        exit 1
    fi
}

requirements()
{

    if [ "$ROOT_PERM" = "yes" ] ; then

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


	# optional but interesting to get a smaller dar_static binary
	xbps-install -y upx || (echo "" && echo "WARNING!" && echo "Failed to install upx, will do without" && echo && sleep 3)

	# openssl needs perl
	xbps-install -y perl || (echo "" && echo "WARNING!" && echo "Failed to install perl" && exit 1)

    fi
}

openssl()
{
    local OPENSSL_PKG=openssl-${OPENSSL}.tar.gz

    if [ ! -e "${REPO}/${OPENSSL_PKG}" ] ; then wget "https://github.com/openssl/openssl/releases/download/openssl-$OPENSSL/${OPENSSL_PKG}" && mv "${OPENSSL_PKG}" "${REPO}" || exit 1 ; fi
    tar -xf "${REPO}/${OPENSSL_PKG}" || exit 1
    cd openssl-${OPENSSL} || exit 1
    ./Configure --prefix="$LOCAL_PREFIX" enable-ktls threads || exit 1
    make ${MAKE_FLAGS} || exit 1
    make install || exit 1
    cd ..
    ldconfig
    rm -rf openssl-${OPENSSL}
}

nlhomann()
{
    mkdir -p "$LOCAL_PREFIX/include/nlohmann" || exit 1
    wget https://raw.githubusercontent.com/nlohmann/json/refs/tags/v3.12.0/single_include/nlohmann/json.hpp || exit 1
    mv json.hpp "$LOCAL_PREFIX/include/nlohmann" || exit 1
}

webdar_static()
{
    make clean || /bin/true
    make distclean || /bin/true
    ./configure --enable-static\
		--disable-shared\
		--enable-webdar-static\
		--prefix="$LOCAL_PREFIX" || exit 1
    make ${MAKE_FLAGS} || exit 1
    make DESTDIR=${tmp_dir} install-strip || exit 1
    mv "$LOCAL_PREFIX"/bin/webdar_static . && echo "webdar_static binary is available in the current directory"
}

check
requirements || (echo "Failed setting up requirements" && exit 1)
openssl || (echo "Failed building libssl" && exit 1)
nlhomann || (echo "Failed installing json lib by Neils Lhomann" && exit 1)
webdar_static || (echo "Failed building webdar_static" && exit 1)
