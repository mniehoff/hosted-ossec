# SIEM as a Service

## Files overview

create_prepared_agent.sh
- Provide $OSSEC_MANAGER_IP in Script
- Install the wazuh agent, authenticate with wazuh master (make sure ossec-authd is running and listening on port 1515)
- Install redis CLI
- Configures cron for tcpdmps3.sh to run every second

install_wazuh_server.sh
- Provide $ELASTIC_HOST, $ELASTIC_USER, $ELASTIC_PASSWD
- Installs wazuh manager
- Provides agent config
- Installs filebeat
- Configures filebeat to provided elasticsearch

install_snort_server.sh
- Provide $REDIS_HOST
- Installs daq and snort
- Configures snort rules
- Runs Redis Client, feeding data into snort?

tcpdmps3.sh
- Provide $REDIS_HOST
- runs tcpdump, uploads pcap files to redis

## Necessary infrastructure

- Host for wazuh & snort (can be one for both)
- Redis for transfering tcpdumps to snort (i.e. AWS elasticache)
- Elasticsearch instance, reachable over http/https (i.e. elastic.io or AWS ES)

## Necessary network ports

Port 1515 tcp: Agent -> Management: Registration
Port 1514 udp or tcp: Agent -> Management: Data
Port 55000 tcp: Manager Rest API
Port 6379 for redis producer/consumer

## Create Elastic Search Schema

curl https://raw.githubusercontent.com/wazuh/wazuh-kibana-app/master/server/startup/integration_files/template_file.json | curl -XPUT -u elastic:e5302mTjgx8WrNo3MwQwA7G8 'https://ac94a37dc92f2e5f971b4d4a1c7b1b97.us-east-1.aws.found.io:9243/_template/wazuh' -H 'Content-Type: application/json' -d @-

## Helpful commands

on wazuh server. print registered agents
`curl -u foo:bar -k http://127.0.0.1:55000/agents?pretty`

manage agents (on manager)
`/var/ossec/bin/manage_agents`

start ossec auth service (on manager)
`/var/ossec/bin/ossec-authd -p 1515 -a >/dev/null 2>&1 &`

register agent
```
export OSSEC_MANAGER_IP="35.157.98.125"
/var/ossec/bin/agent-auth -m "$OSSEC_MANAGER_IP"
/var/ossec/bin/ossec-control restart
```

The OSSEC message protocol uses a 192-bit Blowfish encryption with a full 16-round implementation, at this time has no publicly known cryptographic weaknesses.
The Wazuh server uses Filebeat to ship alert and event data to Logstash (5000/TCP) on the Elastic Stack server, using TLS encryption

Agents/Manager can be verified using ssl certificates.
Authorize agents using passwords
