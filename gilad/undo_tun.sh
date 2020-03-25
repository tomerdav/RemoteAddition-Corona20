#!/bin/bash

route del -net 0.0.0.0 netmask 0.0.0.0 metric 0 dev tun0
