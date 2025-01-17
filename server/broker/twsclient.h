#pragma once

#include "EClientSocket.h"
#include "EWrapper.h"
#include "Contract.h"
#include "Decimal.h"
#include "logger/quantlogger.h"
#include "signalhandler.h"
#include "concurrentqueue/blockingconcurrentqueue.h"

#include <string>
#include <map>
#include <functional>
#include <atomic>
#include <memory>

class EReader;

namespace quanttrader {
namespace broker {

namespace qlog = quanttrader::log;

struct ResponseHeader;
struct ResErrorMsg;

class TwsClient : public EWrapper {
public:
    TwsClient() = delete;
    TwsClient(const std::string_view ip, int port, int clientid, int wait_timeout = 10);
    ~TwsClient();

    bool connect();
    inline bool is_connected() const { return client_socket_.isConnected(); }
    void disconnect();
    void process_messages();
    void update_connect_config(const std::string_view ip, int port, int clientid);
    const std::string_view get_host() const { return host_; }
    int get_port() const { return port_; }
    int get_clientid() const { return clientid_; }
    void set_response_queue(std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<ResponseHeader>>> response_queue) { response_queue_ = response_queue; };
    void set_error_queue(std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<ResErrorMsg>>> error_queue) { error_queue_ = error_queue; };

    // EClient requests
    void request_current_time();
    void request_real_time_data(TickerId request_id, const Contract &contract);
    void cancel_real_time_data(TickerId request_id);
    void request_historical_data(TickerId request_id, const Contract &contract, const std::string &end_time,
                                  const std::string &duration, const std::string &bar_size,
                                  const std::string &what_to_show, int use_rth);
    void cancel_historical_data(TickerId request_id);


    // EWrapper callbacks
    void tickPrice(TickerId ticker_id, TickType field, double price, const TickAttrib &attrib) override;
    void tickSize(TickerId ticker_id, TickType field, Decimal size) override;
    void historicalData(TickerId req_id, const Bar &bar) override;
    void historicalDataEnd(int req_id, const std::string& start_date, const std::string& end_date) override;
    void error(int id, time_t error_time, int error_code, const std::string &error_string, const std::string& advancedOrderRejectJson) override;
    void connectionClosed() override;
    void currentTime(long time) override;

    static TickerId next_request_id() { return next_request_id_.fetch_add(1, std::memory_order_relaxed); }

    // Not implemented EWrapper virtual functions
    virtual void tickOptionComputation( TickerId tickerId, TickType tickType, int tickAttrib, double impliedVol, double delta,
        double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice) override {};
    virtual void tickGeneric(TickerId tickerId, TickType tickType, double value) override {};
    virtual void tickString(TickerId tickerId, TickType tickType, const std::string& value) override {};
    virtual void tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const std::string& formattedBasisPoints,
        double totalDividends, int holdDays, const std::string& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate) override {};
    virtual void orderStatus( OrderId orderId, const std::string& status, Decimal filled,
        Decimal remaining, double avgFillPrice, long long permId, int parentId,
        double lastFillPrice, int clientId, const std::string& whyHeld, double mktCapPrice) override {};
    virtual void openOrder( OrderId orderId, const Contract&, const Order&, const OrderState&) override {};
    virtual void openOrderEnd() override {};
    virtual void winError( const std::string& str, int lastError) override {};
    virtual void updateAccountValue(const std::string& key, const std::string& val,
    const std::string& currency, const std::string& accountName) override {};
    virtual void updatePortfolio( const Contract& contract, Decimal position,
        double marketPrice, double marketValue, double averageCost,
        double unrealizedPNL, double realizedPNL, const std::string& accountName) override {};
    virtual void updateAccountTime(const std::string& timeStamp) override {};
    virtual void accountDownloadEnd(const std::string& accountName) override {};
    virtual void nextValidId( OrderId orderId) override {};
    virtual void contractDetails( int reqId, const ContractDetails& contractDetails) override {};
    virtual void bondContractDetails( int reqId, const ContractDetails& contractDetails) override {};
    virtual void contractDetailsEnd( int reqId) override {};
    virtual void execDetails( int reqId, const Contract& contract, const Execution& execution) override {};
    virtual void execDetailsEnd( int reqId) override {};
    virtual void updateMktDepth(TickerId id, int position, int operation, int side,
        double price, Decimal size) override {};
    virtual void updateMktDepthL2(TickerId id, int position, const std::string& marketMaker, int operation,
        int side, double price, Decimal size, bool isSmartDepth) override {};
    virtual void updateNewsBulletin(int msgId, int msgType, const std::string& newsMessage, const std::string& originExch) override {};
    virtual void managedAccounts( const std::string& accountsList) override {};
    virtual void receiveFA(faDataType pFaDataType, const std::string& cxml) override {};
    virtual void scannerParameters(const std::string& xml) override {};
    virtual void scannerData(int reqId, int rank, const ContractDetails& contractDetails,
        const std::string& distance, const std::string& benchmark, const std::string& projection,
        const std::string& legsStr) override {};
    virtual void scannerDataEnd(int reqId) override {};
    virtual void realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
        Decimal volume, Decimal wap, int count) override {};
    virtual void fundamentalData(TickerId reqId, const std::string& data) override {};
    virtual void deltaNeutralValidation(int reqId, const DeltaNeutralContract& deltaNeutralContract) override {};
    virtual void tickSnapshotEnd( int reqId) override {};
    virtual void marketDataType( TickerId reqId, int marketDataType) override {};
    virtual void commissionAndFeesReport( const CommissionAndFeesReport& commissionAndFeesReport) override {};
    virtual void position( const std::string& account, const Contract& contract, Decimal position, double avgCost) override {};
    virtual void positionEnd() override {};
    virtual void accountSummary( int reqId, const std::string& account, const std::string& tag, const std::string& value, const std::string& currency) override {};
    virtual void accountSummaryEnd( int reqId) override {};
    virtual void verifyMessageAPI( const std::string& apiData) override {};
    virtual void verifyCompleted( bool isSuccessful, const std::string& errorText) override {};
    virtual void displayGroupList( int reqId, const std::string& groups) override {};
    virtual void displayGroupUpdated( int reqId, const std::string& contractInfo) override {};
    virtual void verifyAndAuthMessageAPI( const std::string& apiData, const std::string& xyzChallange) override {};
    virtual void verifyAndAuthCompleted( bool isSuccessful, const std::string& errorText) override {};
    virtual void connectAck() override {};
    virtual void positionMulti( int reqId, const std::string& account,const std::string& modelCode, const Contract& contract, Decimal pos, double avgCost) override {};
    virtual void positionMultiEnd( int reqId) override {};
    virtual void accountUpdateMulti( int reqId, const std::string& account, const std::string& modelCode, const std::string& key, const std::string& value, const std::string& currency) override {};
    virtual void accountUpdateMultiEnd( int reqId) override {};
    virtual void securityDefinitionOptionalParameter(int reqId, const std::string& exchange, int underlyingConId, const std::string& tradingClass,
        const std::string& multiplier, const std::set<std::string>& expirations, const std::set<double>& strikes) override {};
    virtual void securityDefinitionOptionalParameterEnd(int reqId) override {};
    virtual void softDollarTiers(int reqId, const std::vector<SoftDollarTier> &tiers) override {};
    virtual void familyCodes(const std::vector<FamilyCode> &familyCodes) override {};
    virtual void symbolSamples(int reqId, const std::vector<ContractDescription> &contractDescriptions) override {};
    virtual void mktDepthExchanges(const std::vector<DepthMktDataDescription> &depthMktDataDescriptions) override {};
    virtual void tickNews(int tickerId, time_t timeStamp, const std::string& providerCode, const std::string& articleId, const std::string& headline, const std::string& extraData) override {};
    virtual void smartComponents(int reqId, const SmartComponentsMap& theMap) override {};
    virtual void tickReqParams(int tickerId, double minTick, const std::string& bboExchange, int snapshotPermissions) override {};
    virtual void newsProviders(const std::vector<NewsProvider> &newsProviders) override {};
    virtual void newsArticle(int requestId, int articleType, const std::string& articleText) override {};
    virtual void historicalNews(int requestId, const std::string& time, const std::string& providerCode, const std::string& articleId, const std::string& headline) override {};
    virtual void historicalNewsEnd(int requestId, bool hasMore) override {};
    virtual void headTimestamp(int reqId, const std::string& headTimestamp) override {};
    virtual void histogramData(int reqId, const HistogramDataVector& data) override {};
    virtual void historicalDataUpdate(TickerId reqId, const Bar& bar) override {};
    virtual void rerouteMktDataReq(int reqId, int conid, const std::string& exchange) override {};
    virtual void rerouteMktDepthReq(int reqId, int conid, const std::string& exchange) override {};
    virtual void marketRule(int marketRuleId, const std::vector<PriceIncrement> &priceIncrements) override {};
    virtual void pnl(int reqId, double dailyPnL, double unrealizedPnL, double realizedPnL) override {};
    virtual void pnlSingle(int reqId, Decimal pos, double dailyPnL, double unrealizedPnL, double realizedPnL, double value) override {};
    virtual void historicalTicks(int reqId, const std::vector<HistoricalTick> &ticks, bool done) override {};
    virtual void historicalTicksBidAsk(int reqId, const std::vector<HistoricalTickBidAsk> &ticks, bool done) override {};
    virtual void historicalTicksLast(int reqId, const std::vector<HistoricalTickLast> &ticks, bool done) override {};
    virtual void tickByTickAllLast(int reqId, int tickType, time_t time, double price, Decimal size, const TickAttribLast& tickAttribLast, const std::string& exchange, const std::string& specialConditions) override {};
    virtual void tickByTickBidAsk(int reqId, time_t time, double bidPrice, double askPrice, Decimal bidSize, Decimal askSize, const TickAttribBidAsk& tickAttribBidAsk) override {};
    virtual void tickByTickMidPoint(int reqId, time_t time, double midPoint) override {};
    virtual void orderBound(long long permId, int clientId, int orderId) override {};
    virtual void completedOrder(const Contract& contract, const Order& order, const OrderState& orderState) override {};
    virtual void completedOrdersEnd() override {};
    virtual void replaceFAEnd(int reqId, const std::string& text) override {};
    virtual void wshMetaData(int reqId, const std::string& dataJson) override {};
    virtual void wshEventData(int reqId, const std::string& dataJson) override {};
    virtual void historicalSchedule(int reqId, const std::string& startDateTime, const std::string& endDateTime, const std::string& timeZone, const std::vector<HistoricalSession>& sessions) override {};
    virtual void userInfo(int reqId, const std::string& whiteBrandingId) override {};

private:
    SignalHandler signal_handler_; // Signal handler for EClientSocket
    EClientSocket client_socket_;
    std::unique_ptr<EReader> reader_ptr_;
    qlog::LoggerPtr logger_{nullptr};

    std::string host_{""};
    int port_{0};
    int clientid_{0};

    static std::atomic<TickerId> next_request_id_;
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<ResponseHeader>>> response_queue_{nullptr};
    std::shared_ptr<moodycamel::BlockingConcurrentQueue<std::shared_ptr<ResErrorMsg>>> error_queue_{nullptr};
};

}
}