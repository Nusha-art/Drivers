#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>

static struct net_device *demo_nic;

static int demo_nic_open(struct net_device *dev)
{
    pr_info("demo_nic open\n");
    netif_start_queue(dev);
    return 0;
}

static int demo_nic_release(struct net_device *dev)
{
    pr_info("demo_nic release\n");
    netif_stop_queue(dev);
    return 0;
}

static netdev_tx_t demo_nic_xmit(struct sk_buff *skb, struct net_device *dev)
{
    pr_info("demo_nic xmit len=%u\n", skb->len);

    dev->stats.tx_packets++;
    dev->stats.tx_bytes += skb->len;

    dev_kfree_skb(skb);
    return NETDEV_TX_OK;
}

static int demo_nic_init(struct net_device *dev)
{
    pr_info("demo_nic initialized\n");
    return 0;
}

static const struct net_device_ops demo_nic_netdev_ops = {
    .ndo_init       = demo_nic_init,
    .ndo_open       = demo_nic_open,
    .ndo_stop       = demo_nic_release,
    .ndo_start_xmit = demo_nic_xmit,
};

static void demo_nic_setup(struct net_device *dev)
{
    ether_setup(dev);
    dev->netdev_ops = &demo_nic_netdev_ops;
    dev->flags |= IFF_NOARP;
    dev->features |= NETIF_F_HW_CSUM;
}

static int __init demo_nic_module_init(void)
{
    int ret;

    demo_nic = alloc_netdev(0, "demo%d", NET_NAME_UNKNOWN, demo_nic_setup);
    if (!demo_nic)
        return -ENOMEM;

    ret = register_netdev(demo_nic);
    if (ret) {
        free_netdev(demo_nic);
        return ret;
    }

    return 0;
}

static void __exit demo_nic_module_exit(void)
{
    unregister_netdev(demo_nic);
    free_netdev(demo_nic);
}

module_init(demo_nic_module_init);
module_exit(demo_nic_module_exit);

MODULE_LICENSE("GPL");
