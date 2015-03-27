#CheckoutCrypto Worker
![Alt text](https://github.com/CheckoutCrypto/crypto-api/blob/master/control/2fa/images/checkoutcrypto_logo.jpg "CheckoutCrypto logo")

## Table of contents

- [Dependencies](#dependencies)
- [Optionally](#optionally)
- [Docker](#docker)
- [Usage Menu](#usage-menu)
- [Usage Server](#usage-server)
- [Post-installation](#post-installation)
- [Menu](#menu)
- [Explanation](#explanation)
- [Logs](#logs)
- [Bugs](#bugs)
- [License](#license)

###Dependencies

```
sudo apt-get install git qt5-default qt5-qmake libqt5sql5 libqt5sql5-mysql libqt5sql5-sqlite build-essential libmysqlclient-dev monit
git clone https://github.com/CheckoutCrypto/worker
cd ./worker
qmake && make
```
All the above are preinstalled in the docker image

#### Optionally
Create a worker service at /etc/init.d/worker
add worker service to default:

```
sudo /usr/sbin/update-rc.d worker defaults
```

## Docker
### Required Containers
Run MySQL daemon container with mysql-server (exposed port 3306)

```
docker run --name mysql -e MYSQL_ROOT_PASSWORD=somepass -d mysql
```

Run PHPMyAdmin daemon container with mysql connection(exposed port 80 mapped to 81)

```
docker run -d --link mysql:mysql -e MYSL_USERNAME=root --name phpmyadmin -p 81:80 corbinu/docker-phpmyadmin
```

###Usage Menu

```
 docker run -d -it -p 12311:12311 --name worker --link mysql:mysql checkoutcrypto/worker-qt /worker/workServer
```

###Usage Server

```
 docker run -it -p 12311:12311 --name worker --link mysql:mysql checkoutcrypto/worker-qt /worker/workServer -server -pass test
```


###Post-installation

- [CheckoutCrypto Drupal Site and Database](https://registry.hub.docker.com/u/checkoutcrypto/site/) Installed and configured separately.
- [CheckoutCrypto API](https://registry.hub.docker.com/u/checkoutcrypto/crypto-api/)
- [Bitcoin daemon](https://bitcoin.org/en/download)

[Read the repository for more information](https://github.com/CheckoutCrypto/worker) 

###Menu

```
Welcome to the Worker 
********************************
================================
=======  CheckoutCrypto Worker  =========
================================
*********************************
1) Add/Edit Remote Database
2) Add/Edit Coin Daemon
3) Add/Edit API Key
4) Run Server
Enter Choice(0-4): 
```

####Explanation
1. Add/Edit the Site Front-end Database

2. Add/Edit the RPC Information for each cryptocurrency daemon

3. Add/Update the API key, necessary to connect the API -> Worker (ccapiconfig.php in API repo)

4. Start temporary Worker Server

###Start Worker Server
```
/worker/workServer -server -pass YOURPASSWRD   
```

###Listen IP + Port
in ./src/server/server.cpp on line 64
```
host.setAddress("127.0.0.1"); 
```

and on line 68 you can change the port
```
server->listen(host, 12311);
```

If you're running CheckoutCrypto's stack, this will be automatically built to 0.0.0.0

### SQLite Cache
~/.cache/worker2/cache.db

###Log

```
View Log(after running server mode once) with:
docker exec -it worker tail -n 200 /var/log/worker.log
```

###bugs: 
1. memory leak(1 gig after tens of thousands of orders), restart worker with monit.  If you have/know a fix please contact maintainer.

2. password protection is very weak, assuming linux user isn't compromised isn't enough, this needs to be beefed up on any production level server.

##License
[Licensed under Apache 2.0 License](https://github.com/CheckoutCrypto/site/blob/master/COPYRIGHT) with one small reservation.
