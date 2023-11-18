main_PID=$(pgrep -f "1.sh")

PIDs=($(pgrep -P "$main_PID"))

to_kill=$((${#PIDs[@]}-1))


for i in $(seq 0 1 $to_kill); 
do
	kill "${PIDs[i]}"
done

kill $main_PID


