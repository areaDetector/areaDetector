#!/bin/bash

DOXYGEN_VER=1.8.15
DOXYGEN_TAR=${DOXYGEN_VER}.linux.bin.tar.gz
DOXYGEN_URL="https://downloads.sourceforge.net/project/doxygen/rel-${DOXYGEN_VER}/doxygen-${DOXYGEN_VER}.linux.bin.tar.gz"
TMP_DIR=${TMPDIR-/tmp}

echo "Downloading from ${DOXYGEN_URL}"
echo "Installing to ${TMP_DIR}"

wget -O - "${DOXYGEN_URL}" | tar xz -C ${TMP_DIR} doxygen-${DOXYGEN_VER}/bin/doxygen
export PATH="${TMP_DIR}/doxygen-${DOXYGEN_VER}/bin:$PATH"

doxygen --version
