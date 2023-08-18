subnet

Author: Christopher Price
Email: chris@pricedude.com
Version: 1.0
Date: 8/18/23

A simple command-line utilty that will take an IP address and a subnet mask (or CIDR notation)
and output the details of a subnet.

Example 1 (using subnet mask):

          python .\subnet.py 192.168.0.10 255.255.255.0

                  IP Address:  192.168.0.10/24 (255.255.255.0)
             Number of hosts:  254
             Network address:  192.168.0.0
                  First host:  192.168.0.1
                   Last host:  192.168.0.254
           Broadcast address:  192.168.0.255

Example 2 (using CIDR notation):

          python .\subnet.py 192.168.0.10/24

                  IP Address:  192.168.0.10/24 (255.255.255.0)
             Number of hosts:  254
             Network address:  192.168.0.0
                  First host:  192.168.0.1
                   Last host:  192.168.0.254
           Broadcast address:  192.168.0.255
