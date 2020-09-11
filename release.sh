rel=v0.5.1.1
mkdir release >/dev/null 2>&1
rm -rf release/$rel >/dev/null 2>&1
mkdir release/$rel
cp instrument-panel/instrument-panel release/$rel
cp -rp instrument-panel/bitmaps release/$rel
cp -rp instrument-panel/settings release/$rel
tar -zcvf release/instrument-panel-$rel-raspi4.tar.gz release/$rel
