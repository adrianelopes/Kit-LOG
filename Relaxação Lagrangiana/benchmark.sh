make

k=1

for instance in instances/*; do
	echo $instance >> ./BFS.txt
	
	echo "BFS"
	echo "Processando $instance"
	echo "Instance $k of 15"
	
	./tsp ${instance} 2 >> ./BFS.txt
	
	k=$(($k+1))
done

echo "-" >> ./BFS.txt

