to_kill=3

PIDs=($(pgrep -P $(pgrep -f "1.sh")))

for i in $(seq 1 1 $to_kill); 
do
	kill "${PIDs[i]}"
done
