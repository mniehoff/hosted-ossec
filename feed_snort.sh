export REDIS_HOST="siem.8pwkcy.0001.euc1.cache.amazonaws.com"

echo "chk redis"
  LIST="$(/usr/local/bin/redis-cli -h "$REDIS_HOST" KEYS "trace*pcap" | awk '{print $1}')"
while read -r p; do

    if [ "x$p" == "x" ]; then
       break
    fi

  echo "proc $p"
  /usr/local/bin/redis-cli -h "$REDIS_HOST" --raw HGET "$p" pcap | head -c-1 | /usr/local/bin/snort -c /etc/snort/snort.conf -r - > /dev/null 2>&1 \
  && /usr/local/bin/redis-cli -h "$REDIS_HOST" del "$p" > /dev/null && echo "  - Success"
done <<< "$LIST"

echo "/var/log/snort/snort_alert.full $(cat /var/log/snort/snort_alert.full)"

sleep 5
