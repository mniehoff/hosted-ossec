#!/bin/bash
set -e
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
sudo yum -y install wget
wget https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
sudo rpm -Uvh epel-release-latest-7*.rpm
#yum -y install make gcc git flex bison libpcap-devel pcre-devel libdnet-devel.x86_64 zlib-devel libnghttp2-devel xz-devel tcpdump --enablerepo=epel
yum -y install make gcc git flex bison zlib libpcap pcre libdnet tcpdump xz
yum -y install https://dl.fedoraproject.org/pub/epel/7/x86_64/l/libnghttp2-1.21.1-1.el7.x86_64.rpm
#https://www.upcloud.com/support/installing-snort-on-centos/
export REDIS_HOST="ossecredis.jyu98g.0001.usw2.cache.amazonaws.com"

cd $DIR/
wget https://www.snort.org/downloads/snort/daq-2.0.6.tar.gz
wget https://www.snort.org/downloads/snort/snort-2.9.9.0.tar.gz

tar xvfz daq-2.0.6.tar.gz
tar xvfz snort-2.9.9.0.tar.gz

cd $DIR/daq-2.0.6
./configure && make && make install

cd $DIR/snort-2.9.9.0
./configure --enable-sourcefire && make && make install

cd $DIR/

ldconfig

ln -s /usr/local/bin/snort /usr/sbin/snort

groupadd snort
useradd snort -r -s /sbin/nologin -c SNORT_IDS -g snort

mkdir /etc/snort
mkdir /etc/snort/rules
mkdir /usr/local/lib/snort_dynamicrules
mkdir /var/log/snort

touch /etc/snort/rules/white_list.rules /etc/snort/rules/black_list.rules /etc/snort/rules/local.rules
chmod -R 5775 /etc/snort
chmod -R 5775 /var/log/snort
chmod -R 5775 /usr/local/lib/snort_dynamicrules
chown -R snort:snort /etc/snort
chown -R snort:snort /var/log/snort
chown -R snort:snort /usr/local/lib/snort_dynamicrules

cp /snort*/etc/*.conf* /etc/snort
cp /snort*/etc/*.map /etc/snort

#wget https://www.snort.org/rules/community -O /community.tar.gz
#tar -xvf /community.tar.gz -C /
#cp /community-rules/* /etc/snort/rules
#sed -i 's/include \$RULE\_PATH/#include \$RULE\_PATH/' /etc/snort/snort.conf

cd "$DIR/snort_rules"
cp -a * /etc/

cd /


cp -f "$DIR/snort.conf.tpl" /etc/snort/snort.conf

snort -T -c /etc/snort/snort.conf

# Add cron job for feeding script

crontab -l > /tmp/rootcron.tmp 2>/dev/null

if [[ $(cat /tmp/rootcron.tmp) != *"tcpdmps3"* ]]
then
    echo "* * * * * /usr/bin/flock -w 600 /var/feedsnort.lock $DIR/feed_snort.sh" >> /tmp/rootcron.tmp
    crontab /tmp/rootcron.tmp
else
    :
fi

rm -rf /tmp/rootcron.tmp

#trace-h2-ip-172-31-2-100-2017-03-24_14:05:11.pcap
#trace-h2-ip-172-31-2-100-2017-03-24_14:38:16.pcap
#trace-h2-ip-172-31-2-100-2017-03-24_14:19:03.pcap
