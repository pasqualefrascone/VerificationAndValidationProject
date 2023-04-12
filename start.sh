#!/bin/bash

docker compose up -d server
docker compose up -d client
#docker compose run client ./client CLIENTUNICO

sleep 11
cat numb | docker exec -i client1 ./client CLIENT1 & >/dev/null
#cat numbers2 | docker exec -i client1 ./client CLIENT2 &
#cat numbers1 | docker exec -i client1 ./client CLIENT3 &
#cat numbers2 | docker exec -i client1 ./client CLIENT4 &
#cat numbers1 | docker exec -i client1 ./client CLIENT5 &
#cat numbers2 | docker exec -i client1 ./client clientACaso &
#cat numbers1 | docker exec -i client1 ./client CLIENT6 &
#cat numbers2 | docker exec -i client1 ./client CLIENT7 &
#cat numbers1 | docker exec -i client1 ./client CLIENT8 &
#cat numbers2 | docker exec -i client1 ./client RANDOMCLIENT &


#dig -x `ifconfig eth0 | grep 'inet' | awk '{print $2}'` +short | cut -d'.' -f1