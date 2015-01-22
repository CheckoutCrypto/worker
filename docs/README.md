CheckoutCrypto Worker
==========
```
sudo apt-get install git qt5-default qt5-qmake libqt5sql5 libqt5sql5-mysql libqt5sql5-sqlite build-essential libmysqlclient-dev monit
git clone https://github.com/CheckoutCrypto/worker
cd ./worker
qmake && make
Create a worker service at /etc/init.d/worker
add worker service to default:
sudo /usr/sbin/update-rc.d worker defaults
```

<h2>Menu</h2>
=======
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

<h3>Explanation</h3>
<ol>
<li>Add/Edit the Site Front-end Database </li>
<li>Add/Edit the RPC Information for each cryptocurrency daemon</li>
<li>Add/Update the API key, necessary to connect the API -> Worker (ccapiconfig.php in API repo)</li>
<li>Start temporary Worker Server</li>
</ol>  

<h3>Start Worker Server</h3>
```
cd ./worker
./workServer -server -pass YOURPASSWRD   
```
The password is weak, visible, breakable, not a good option for security at the moment. Ideally in the near future, I'd like to make better use of encryption libraries, for dealing with the rpc information in cache.  That is the only reason for the password, it provides very basic encryption instead of text based (e.g. your skype message cache). An assumption is made that your linux user account is the master password for all your currencies.  Certainly not the most secure method for production.

<h3>Cache</h3>
~/.cache/worker2/cache.db

<h3>Log</h3>
```
sudo touch /var/log/worker.log
sudo chown ccuser:ccuser /var/log/worker.log   ( or anyother user/group )
sudo chmod 777 /var/log/worker.log

View Log with:
sudo tail -n 200 /var/log/worker.log
```

<h3>known bugs: </h3>
<ol>
<li>memory leak(1 gig after tens of thousands of orders), restart worker with monit.  If you have/know a fix please contact maintainer.</li>
<li>password protection is very weak, assuming linux user isn't compromised isn't enough, this needs to be beefed up on any production level server.</li>
</ol>
