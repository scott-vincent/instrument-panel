rel=v1.7.1
mkdir release >/dev/null 2>&1
rm -rf release/$rel >/dev/null 2>&1
mkdir release/$rel
cp instrument-panel/instrument-panel release/$rel
cp -rp instrument-panel/bitmaps release/$rel
cp -rp instrument-panel/settings release/$rel
sudo chown pi:pi release/$rel/settings/*.json
dos2unix release/$rel/settings/*.json
cp release/$rel/settings/default-settings.json release/$rel/settings/instrument-panel.json
tar -zcvf release/instrument-panel-$rel-raspi4.tar.gz release/$rel
