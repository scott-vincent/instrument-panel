rel=v2.0.1
mkdir release >/dev/null 2>&1
rm -rf release/$rel >/dev/null 2>&1
mkdir release/$rel
cp instrument-panel/instrument-panel release/$rel
cp -rp instrument-panel/bitmaps release/$rel
cp -rp instrument-panel/settings release/$rel
sudo chown pi:pi release/$rel/settings/*.json
dos2unix release/$rel/settings/*.json
cd release
cp $rel/settings/default-settings.json $rel/settings/instrument-panel.json
tar -zcvf instrument-panel-$rel-raspi5-64.tar.gz $rel
