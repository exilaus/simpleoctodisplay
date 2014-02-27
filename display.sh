curl -s "http://localhost/api/state?apikey=your_key" > info.txt

STATE=$(cat info.txt| jq '.state.state')
if [ "$STATE" = "null" ]
then
./Displayprn
else
HE=$(cat info.txt| jq '.temperature.extruder.current')
HB=$(cat info.txt| jq '.temperature.bed.current')
ETA=$(cat info.txt| jq '.progress.printTimeLeft')
PROG=$(cat info.txt| jq '.progress.progress')
FILE=$(cat info.txt| jq '.currentZ.job.filename')

./Displayprn MAKIBOX_Stat: HE:$HE-HB:$HB ETA:$ETA File:$FILE COMPLETE:$PROG LINEFREE_EXI

fi
