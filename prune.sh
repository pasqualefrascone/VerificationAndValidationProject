sudo docker compose down
sudo docker container ls -a | awk '{print $1}' | while read line; do sudo docker volume rm -f $line; done
sudo docker volume ls | awk '{print $2}' | while read line; do sudo docker volume rm $line; done
sudo docker image ls | awk '{print $3}' | while read line; do sudo docker rmi $line; done
sudo docker system prune