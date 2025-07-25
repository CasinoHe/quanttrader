/* Copyright (C) 2025 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package com.ib.client;

public class Execution {
    private int 	m_orderId;
    private int 	m_clientId;
    private String 	m_execId;
    private String 	m_time;
    private String 	m_acctNumber;
    private String 	m_exchange;
    private String 	m_side;
    private Decimal m_shares;
    private double 	m_price;
    private long    m_permId;
    private int     m_liquidation;
    private Decimal m_cumQty;
    private double	m_avgPrice;
    private String  m_orderRef;
    private String 	m_evRule;
    private double 	m_evMultiplier;
    private String  m_modelCode;
    private Liquidities     m_lastLiquidity;
    private boolean m_pendingPriceRevision;
    private String  m_submitter;
    private OptionExerciseType m_optExerciseOrLapseType;

    // Get
    public int orderId()         { return m_orderId; }
    public int clientId()        { return m_clientId; }
    public String execId()       { return m_execId; }
    public String time()         { return m_time; }
    public String acctNumber()   { return m_acctNumber; }
    public String exchange()     { return m_exchange; }
    public String side()         { return m_side; }
    public Decimal shares()      { return m_shares; }
    public double price()        { return m_price; }
    public long permId()         { return m_permId; }
    public int liquidation()     { return m_liquidation; }
    public Decimal cumQty()      { return m_cumQty; }
    public double avgPrice()     { return m_avgPrice; }
    public String orderRef()     { return m_orderRef; }
    public String evRule()       { return m_evRule; }
    public double evMultiplier() { return m_evMultiplier; }
    public String modelCode()    { return m_modelCode; }
    public Liquidities lastLiquidity()   { return m_lastLiquidity; }
    public String lastLiquidityStr()     { return m_lastLiquidity.toString(); }
    public boolean pendingPriceRevision() { return m_pendingPriceRevision; }
    public String submitter()    { return m_submitter; }
    public OptionExerciseType optExerciseOrLapseType() { return m_optExerciseOrLapseType; }
    
    // Set 
    public void orderId(int orderId)              { m_orderId = orderId; }
    public void clientId(int clientId)            { m_clientId = clientId; }
    public void execId(String execId)             { m_execId = execId; }
    public void time(String time)                 { m_time = time; }
    public void acctNumber(String acctNumber)     { m_acctNumber = acctNumber; }
    public void exchange(String exchange)         { m_exchange = exchange; }
    public void side(String side)                 { m_side = side; }
    public void shares(Decimal shares)            { m_shares = shares; }
    public void price(double price)               { m_price = price; }
    public void permId(long permId)               { m_permId = permId; }
    public void liquidation(int liquidation)      { m_liquidation = liquidation; }
    public void cumQty(Decimal cumQty)            { m_cumQty = cumQty; }
    public void avgPrice(double avgPrice)         { m_avgPrice = avgPrice; }
    public void orderRef(String orderRef)         { m_orderRef = orderRef; }
    public void evRule(String evRule)             { m_evRule = evRule; }
    public void evMultiplier(double evMultiplier) { m_evMultiplier = evMultiplier; }
    public void modelCode(String modelCode)       { m_modelCode = modelCode; }
    public void lastLiquidity(int v)              { m_lastLiquidity = Liquidities.fromInt(v); }
    public void pendingPriceRevision(boolean pendingPriceRevision) { m_pendingPriceRevision = pendingPriceRevision; }
    public void submitter(String submitter)       { m_submitter = submitter; }
    public void optExerciseOrLapseType(OptionExerciseType optExerciseOrLapseType) { m_optExerciseOrLapseType = optExerciseOrLapseType; }
    
    public Execution() {
        m_orderId = 0;
        m_clientId = 0;
        m_shares = Decimal.ZERO;
        m_price = 0;
        m_permId = 0;
        m_liquidation = 0;
        m_cumQty = Decimal.ZERO;
        m_avgPrice = 0;
        m_evMultiplier = 0;
        m_lastLiquidity = Liquidities.None;
        m_pendingPriceRevision = false;
        m_optExerciseOrLapseType = OptionExerciseType.None;
    }

    public Execution( int p_orderId, int p_clientId, String p_execId, String p_time,
                      String p_acctNumber, String p_exchange, String p_side, Decimal p_shares,
                      double p_price, long p_permId, int p_liquidation, Decimal p_cumQty,
                      double p_avgPrice, String p_orderRef, String p_evRule, double p_evMultiplier,
                      String p_modelCode, Liquidities lastLiquidity, boolean pendingPriceRevision, String p_submitter,
                      OptionExerciseType p_optExerciseOrLapseType) {
        m_orderId = p_orderId;
        m_clientId = p_clientId;
        m_execId = p_execId;
        m_time = p_time;
      	m_acctNumber = p_acctNumber;
      	m_exchange = p_exchange;
      	m_side = p_side;
      	m_shares = p_shares;
      	m_price = p_price;
        m_permId = p_permId;
        m_liquidation = p_liquidation;
        m_cumQty = p_cumQty;
        m_avgPrice = p_avgPrice;
        m_orderRef = p_orderRef;
        m_evRule = p_evRule;
        m_evMultiplier = p_evMultiplier;
        m_modelCode = p_modelCode;
        m_lastLiquidity = lastLiquidity;
        m_pendingPriceRevision = pendingPriceRevision;
        m_submitter = p_submitter;
        m_optExerciseOrLapseType = p_optExerciseOrLapseType;
    }

    @Override
    public boolean equals(Object p_other) {
        if (this == p_other) {
            return true;
        }
        if (!(p_other instanceof Execution)) {
            return false;
        }
        Execution l_theOther = (Execution)p_other;
        return m_execId.equals(l_theOther.m_execId);
    }

    @Override
    public int hashCode() {
        // Since equals() uses m_execId only, the hashCode should do as well.
        return m_execId != null ? m_execId.hashCode() : 0;
    }
}
