#!/bin/bash

DOXYGEN_VER=doxygen-1.8.15
DOXYGEN_TAR=${DOXYGEN_VER}.linux.bin.tar.gz
DOXYGEN_URL="http://doxygen.nl/files/${DOXYGEN_TAR}"

wget -O - "${DOXYGEN_URL}" | tar xz -C ${TMPDIR-/tmp} ${DOXYGEN_VER}/bin/doxygen
export PATH="${TMPDIR-/tmp}/${DOXYGEN_VER}/bin:$PATH"

