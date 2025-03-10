#!/bin/sh
cd /cleanrip
bear -- make -j`nproc`

cp ./cleanrip.dol ./release/apps/CleanRip/boot.dol
cd ./release/
zip -r /cleanrip/cleanrip.zip .
