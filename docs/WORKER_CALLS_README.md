Worker Explanation
==================

The worker is able to connect to as many cryptocurrencies as necessary(at once), using multiple threads, using multiple lists of orders(from multiple sources).

This worker is the 3rd working prototype of a crypto work server, since Feb 2014.  This worker's base code has been selected for continued improvements because 1) it's cleaner 2) it's utlizing multiple threads and handling multiple sockets much better.

<h3>Configuration</h3>
The configuration is simple.

1. Input Site Front End DB credentials
2. Input Daemon RPC credentials
3. Generate an api key, for connecting from api to worker(./config/ccapiconfig.php in API repo).
4. start the server

<h3>Threads</h3>
There are 3 constant asynchronous threads, with the written, tested possibility of running ever more threads, each running many more threads to conduct work.

Tables: 

ccdev_work_orders, 
ccdev_trade_orders, 
ccdev_cron_orders


Most basic API orders(not a trade or cron) are read from ccdev_work_orders
A successful work order, will give a result = 1 and status = 1

uid = drupal user id
gid = ccdev_groups, the pricing/billing group the user is apart of

<h3>Actions</h3>

There are many types of actions:
	<ol>
		<li>getaddressinfo</li>
		<li>getbalance</li>
		<li>gettransaction</li>
		<li>getreceivedbyaddress</li>
		<li>getnewaddress</li>
		<li>send</li>
		<li>getstatus</li>
		<li>getbalance</li>
		<li>refreshworker</li>
		<li>refreshcoins</li>
		<li>getrate</li>
		<li>servicecharge</li>
		<li>sendfunds</li>
		<li>gettradeaddress</li>
		<li>gettradereceived</li>
		<li>gettradestatus</li>
	</ol>

How worker actually processes the work, it's a bit complex. When it's explained briefly, it helps me at least, looking at the diagram below:

![Alt text](./Worker_Diagram.png?raw=true "CheckoutCrypto Worker Diagram")

The worker has a menu system, specifically for ensuring all cache variables are set before it "works" its orders. (menu.cpp) This is displayed by default when launched with "./workServer".

The worker can be launched, as a server alone, using "./workServer -server -pass YOURPASS"

On server start, worker grabs the cache, sets txfee of worker, checks for any orders, waits for any socket connection.

Those socket connections are only obtained, when a socket connection is opened by the API, utilizing the worker's generated Key, pointing to the thread worker should use.

The worker then creates a new thread(only if one of that same type isn't already working), calls the database assosciated with that thread type (work_orders, trade_orders, cron_orders), grabs all the works, prepares thread by launching several other threads ( orderThread, rpcThread, parseThread ).  The reason we launch these new threads, is because they rely on new connections with different latencies. e.g. one thread for making all daemon calls, one thread for parsing those connections(mySql updates after), one thread just to control which order we're doing and specifications for the order.













