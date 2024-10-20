/*
 Copyright © 2024 王孝慈

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __Simple_VGBE_H__
#define __Simple_VGBE_H__

#define MBit 1000000

enum {
    eePowerStateOff = 0,
    eePowerStateOn,
    eePowerStateCount
};

enum {
    kSpeed2500MBit = 2500*MBit,
    kSpeed1000MBit = 1000*MBit,
    kSpeed100MBit = 100*MBit,
    kSpeed10MBit = 10*MBit,
};

enum {
    kFlowControlTypeNone = 0,
    kFlowControlTypeRx = 1,
    kFlowControlTypeTx = 2,
    kFlowControlTypeRxTx = 3,
    kFlowControlTypeCount
};

enum {
    kEEETypeNo = 0,
    kEEETypeYes = 1,
    kEEETypeCount
};


enum
{
    MEDIUM_INDEX_AUTO = 0,
    MEDIUM_INDEX_10HD,
    MEDIUM_INDEX_10FD,
    MEDIUM_INDEX_100HD,
    MEDIUM_INDEX_100FD,
    MEDIUM_INDEX_100FDFC,
    MEDIUM_INDEX_1000FD,
    MEDIUM_INDEX_1000FDFC,
    MEDIUM_INDEX_2500FD,
    MEDIUM_INDEX_2500FDFC,
    MEDIUM_INDEX_1000FDEEE,
    MEDIUM_INDEX_1000FDFCEEE,
    MEDIUM_INDEX_100FDEEE,
    MEDIUM_INDEX_100FDFCEEE,
    MEDIUM_INDEX_2500FDEEE,
    MEDIUM_INDEX_2500FDFCEEE,
    MEDIUM_INDEX_COUNT
};

class SimpleVGBE: public IOEthernetController {
    OSDeclareDefaultStructors(SimpleVGBE);
    typedef IOEthernetController super;
public:
    // --------------------------------------------------
    // IOService (or its superclass) methods.
    // --------------------------------------------------
    
    virtual bool start(IOService * provider) override;
    virtual void stop(IOService * provider) override ;
    virtual bool init(OSDictionary *properties)override;
    virtual void free() override;

    // --------------------------------------------------
    // Power Management Support
    // --------------------------------------------------
    virtual IOReturn registerWithPolicyMaker(IOService* policyMaker) override;
    virtual IOReturn setPowerState( unsigned long powerStateOrdinal, IOService *policyMaker ) override;
    virtual void systemWillShutdown(IOOptionBits specifier) override;
    
    // --------------------------------------------------
    // IONetworkController methods.
    // --------------------------------------------------
    
    virtual IOReturn enable(IONetworkInterface * netif) override;
    virtual IOReturn disable(IONetworkInterface * netif) override;
    
    virtual UInt32 outputPacket(mbuf_t m, void * param) override;
    
    virtual void getPacketBufferConstraints(IOPacketBufferConstraints * constraints) const override;
#ifndef __PRIVATE_SPI__
    virtual IOOutputQueue * createOutputQueue() override;
#endif
    virtual const OSString * newVendorString() const override;
    virtual const OSString * newModelString() const override;
    
    virtual IOReturn selectMedium(const IONetworkMedium * medium) override;
    virtual bool configureInterface(IONetworkInterface * interface) override;
    
    virtual bool createWorkLoop() override;
    virtual IOWorkLoop * getWorkLoop() const override;
    
    //-----------------------------------------------------------------------
    // Methods inherited from IOEthernetController.
    //-----------------------------------------------------------------------
    
    virtual IOReturn getHardwareAddress(IOEthernetAddress * addr) override;
    virtual IOReturn setHardwareAddress(const IOEthernetAddress * addr) override;
    virtual IOReturn setPromiscuousMode(bool active) override;
    virtual IOReturn setMulticastMode(bool active) override;
    virtual IOReturn setMulticastList(IOEthernetAddress * addrs, UInt32 count) override;
    virtual IOReturn getChecksumSupport(UInt32 *checksumMask, UInt32 checksumFamily, bool isOutput) override;
    virtual IOReturn setMaxPacketSize (UInt32 maxSize) override;
    virtual IOReturn getMaxPacketSize (UInt32 *maxSize) const override;
    virtual IOReturn getMinPacketSize (UInt32 *minSize) const override;
    virtual IOReturn setWakeOnMagicPacket(bool active) override;
    virtual IOReturn getPacketFilters(const OSSymbol * group, UInt32 * filters) const override;
    virtual UInt32 getFeatures() const override;
#ifdef __PRIVATE_SPI__
    virtual IOReturn outputStart(IONetworkInterface *interface, IOOptionBits options) override;
    virtual bool setLinkStatus(
                           UInt32                  status,
                           const IONetworkMedium * activeMedium = 0,
                           UInt64                  speed        = 0,
                           OSData *                data         = 0) override;
#endif
    IOInterruptEventSource * interruptSource;
private:
    IOWorkLoop* workLoop;
    IOPCIDevice* pdev;
    OSDictionary * mediumDict;
    IONetworkMedium * mediumTable[MEDIUM_INDEX_COUNT];
#ifndef __PRIVATE_SPI__
    IOOutputQueue * transmitQueue;
#endif
    
    IOTimerEventSource * watchdogSource;
    IOTimerEventSource * resetSource;
    IOTimerEventSource * dmaErrSource;

    IOEthernetInterface * netif;
    IONetworkStats * netStats;
    IOEthernetStats * etherStats;

    IOMemoryMap * csrPCIAddress;
    
    IOMbufNaturalMemoryCursor * txMbufCursor;
    
    int multicastListCount;

    bool enabledForNetif;
    bool bSuspended;
    bool useTSO;

    bool linkUp;
#ifdef __PRIVATE_SPI__
    //bool polling;
#else
    bool stalled;
#endif

    UInt16 eeeMode;

    UInt32 iff_flags;
    UInt32 _features;
    UInt32 preLinkStatus;
    unsigned long powerState;
    UInt32 _mtu;
    SInt32 txNumFreeDesc;

    UInt32 chip_idx;

    struct igc_adapter priv_adapter;
public:
#ifndef __PRIVATE_SPI__
    bool Stalled() const { return stalled; }
#endif
    void startTxQueue();
    void stopTxQueue();
    UInt32 mtu() { return _mtu; }
    UInt32 flags(){ return iff_flags;}
    UInt32 features() { return _features; }
    igc_adapter* adapter(){ return &priv_adapter; }
    IONetworkStats* getNetStats(){ return netStats; }
    IOEthernetStats* getEtherStats() { return etherStats; }
    int getMulticastListCount() const { return multicastListCount; }
    //dma_addr_t mapSingle( mbuf_t );
    void receive(mbuf_t skb );
    void flushInputQueue();
    void setVid(mbuf_t skb, UInt16 vid);
    IOMbufNaturalMemoryCursor * txCursor(){ return txMbufCursor; }
    void rxChecksumOK( mbuf_t, UInt32 flag );
    bool running(){return enabledForNetif;}
#ifndef __PRIVATE_SPI__
    bool queueStopped(){return txMbufCursor == NULL || stalled;}
#endif
    bool carrier();
    void setCarrier(bool);
    
    void setTimers(bool enable);
private:
    void interruptOccurred(IOInterruptEventSource * src, int count);
    
    void watchdogTask();
    //void updatePhyInfoTask();
    
    int currentMediumIndex();

    void intelRestart();
    bool intelCheckLink(struct igc_adapter *adapter);
    void setLinkUp();
    void setLinkDown();
    void checkLinkStatus();

    UInt16 intelSupportsEEE(struct igc_adapter *adapter);
    bool setupMediumDict();

    void intelSetupAdvForMedium(const IONetworkMedium *medium);
    //bool addNetworkMedium(UInt32 type, UInt32 bps, UInt32 index);

    bool initEventSources( IOService* provider );

    bool igc_probe();
    void igc_remove();

    bool getBoolOption(const char *name, bool defVal);
    int getIntOption(const char *name, int defVal, int maxVal, int minVal );

public:
    static void interruptHandler(OSObject * target,
                                 IOInterruptEventSource * src,
                                 int count );
    
    

    static void watchdogHandler(OSObject * target, IOTimerEventSource * src);
    static void resetHandler(OSObject * target, IOTimerEventSource * src);

};

#endif
