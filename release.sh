rel=v1.3.4
mkdir release >/dev/null 2>&1
rm -rf release/$rel >/dev/null 2>&1
mkdir release/$rel
cp instrument-panel/instrument-panel release/$rel
cp -rp instrument-panel/bitmaps release/$rel
cp -rp instrument-panel/settings release/$rel
sudo chown pi:pi instrument-panel/settings/*.json
dos2unix instrument-panel/settings/*.json
cp instrument-panel/settings/default-settings.json instrument-panel/settings/instrument-panel.json
tar -zcvf release/instrument-panel-$rel-raspi4.tar.gz release/$rel
