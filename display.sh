curl -s "http://localhost/api/state?apikey=your_key" > info.txt

STATE=$(cat info.txt| jq '.state.stateString')
if [ "$STATE" = "Printing" ]
then
./Displayprn
else
HE=$(cat info.txt| jq '.temperature.extruder.current')
HB=$(cat info.txt| jq '.temperature.bed.current')
ETA=$(cat info.txt| jq '.progress.printTimeLeft')
PROG=$(cat info.txt| jq '.progress.progress')
FILE=$(cat info.txt| jq '.job.filename')

./Displayprn MAKIBOX_Stat: HE:$HE-HB:$HB ETA:$ETA File:$FILE COMPLETE:$PROG LINEFREE_EXI

fi
