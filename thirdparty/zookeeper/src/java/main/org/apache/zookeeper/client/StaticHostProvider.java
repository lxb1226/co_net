/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.zookeeper.client;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * Most simple HostProvider, resolves only on instantiation.
 * 
 */
public final class StaticHostProvider implements HostProvider {
    private static final Logger LOG = LoggerFactory
            .getLogger(StaticHostProvider.class);

    private final List<InetSocketAddress> serverAddresses = new ArrayList<InetSocketAddress>(
            5);

    private int lastIndex = -1;

    private int currentIndex_ = -1;

    /**
     * Constructs a SimpleHostSet.
     * 
     * @param serverAddresses
     *            possibly unresolved ZooKeeper server addresses
     * @throws UnknownHostException
     * @throws IllegalArgumentException
     *             if serverAddresses is empty or resolves to an empty list
     */
    public StaticHostProvider(Collection<InetSocketAddress> serverAddresses)
            throws UnknownHostException {
        for (InetSocketAddress address : serverAddresses) {
            InetAddress ia = address.getAddress();
            InetAddress resolvedAddresses[] = InetAddress.getAllByName((ia!=null) ? ia.getHostAddress():
                address.getHostName());
            for (InetAddress resolvedAddress : resolvedAddresses) {
                // If hostName is null but the address is not, we can tell that
                // the hostName is an literal IP address. Then we can set the host string as the hostname
                // safely to avoid reverse DNS lookup.
                // As far as i know, the only way to check if the hostName is null is use toString().
                // Both the two implementations of InetAddress are final class, so we can trust the return value of
                // the toString() method.
                if (resolvedAddress.toString().startsWith("/") 
                        && resolvedAddress.getAddress() != null) {
                    this.serverAddresses.add(
                            new InetSocketAddress(InetAddress.getByAddress(
                                    address.getHostName(),
                                    resolvedAddress.getAddress()), 
                                    address.getPort()));
                } else {
                    this.serverAddresses.add(new InetSocketAddress(resolvedAddress.getHostAddress(), address.getPort()));
                }  
            }
        }
        
        if (this.serverAddresses.isEmpty()) {
            throw new IllegalArgumentException(
                    "A HostProvider may not be empty!");
        }
        Collections.shuffle(this.serverAddresses);
    }

    public int size() {
        return serverAddresses.size();
    }

    public InetSocketAddress next(long spinDelay) {
        ++currentIndex_;
        if (currentIndex_ == serverAddresses.size()) {
            currentIndex_ = 0;
        }
        if (currentIndex_ == lastIndex && spinDelay > 0) {
            try {
                Thread.sleep(spinDelay);
            } catch (InterruptedException e) {
                LOG.warn("Unexpected exception", e);
            }
        } else if (lastIndex == -1) {
            // We don't want to sleep on the first ever connect attempt.
            lastIndex = 0;
        }

        return serverAddresses.get(currentIndex_);
    }

    public void onConnected() {
        lastIndex = currentIndex_;
    }
}
