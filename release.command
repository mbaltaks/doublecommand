#!/bin/sh

# DoubleCommand release script

# - Update version in dcversion.
# - Make sure the Download links are right, and check the news and version history sections.
# - Run this script.

make release
version=`cat dcversion`
git commit -a -m "commit for Release-${version}"
git tag -a Release-${version} -m "Release-${version}"
git push --all --tags
./upload.command

# Now go update versiontracker.com, macupdate.com and send email to mailing list.
