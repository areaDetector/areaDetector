#!/bin/bash

DOXYGEN_VER=1.8.15
DOXYGEN_TAR=${DOXYGEN_VER}.linux.bin.tar.gz
DOXYGEN_URL="https://downloads.sourceforge.net/project/doxygen/rel-${DOXYGEN_VER}/doxygen-${DOXYGEN_VER}.src.tar.gz"
TMP_DIR=${TMPDIR-/tmp}
INSTALL_DIR=${TMP_DIR}/doxygen

echo "Downloading from ${DOXYGEN_URL}"
echo "Building from ${TMP_DIR}"
echo "Installing to ${INSTALL_DIR}"

wget -O - "${DOXYGEN_URL}" | tar xzf - -C ${TMP_DIR}

cd ${TMP_DIR}/doxygen-${DOXYGEN_VER}
mkdir build && cd build

cmake -G "Unix Makefiles" \
	-DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
	..

make
make install
