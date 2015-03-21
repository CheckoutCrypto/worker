FROM ubuntu:14.04
MAINTAINER Grant Hutchinson <h.g.utchinson@gmail.com>


RUN apt-get update && \
    apt-get install -y git qt5-default qt5-qmake libqt5sql5 libqt5sql5-mysql libqt5sql5-sqlite libmysqlclient-dev build-essential monit && \
	git clone https://github.com/CheckoutCrypto/worker  && cd /worker && \
	qmake && make && \
    apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*


EXPOSE 12311

WORKDIR /worker











