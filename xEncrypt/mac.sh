#!/usr/bin/env bash
# Get the current directory this script is found in.
BASEDIR=$(dirname $0)

echo "Encrypting Assets (Mac) : Begin"

# Create an uncompressed archive file.
/Applications/Keka.app/Contents/Resources/keka7z a -tzip -p14611769812890929240 -r -mx9 -mm=Deflate -mfb=128 -mpass10 -mmt=on ${BASEDIR}/../Resources/data ${BASEDIR}/../xScripts/ > /dev/null;

# Delete the current file found in the Resources folder.
rm ${BASEDIR}/../Resources/evz.archive > /dev/null;

# Rename out newest data archive.
mv ${BASEDIR}/../Resources/data.zip ${BASEDIR}/../Resources/evz.archive > /dev/null;

# Done
echo "Encrypting Assets (Mac) : Complete"
