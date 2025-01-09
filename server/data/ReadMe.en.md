# Implementation of the Data Module

The functional requirement of the data module is to be able to obtain historical data and real-time transaction data, and to be able to pass them to third-party libraries such as ta-lib for indicator calculation.

## Detailed Requirements

1. Data storage, compression, and operation requirements. To perform data backtesting and forecasting, relatively fast data access is required. Backtesting within an application may involve restarts, and to minimize the time required to fetch data from the network, data needs to be stored. Due to hard drive size limitations, data should be able to be compressed and easily previewed, with the ability to delete unnecessary cached data.
2. Data modification requirements. During strategy calculation and backtesting, a certain degree of forecasting will be performed, and the forecasting process generates data. This data is not real data and will be reverted after testing. This data might be used for further backtesting, so it also needs to be stored, compressed, and modifiable.
3. Data acquisition. The ability to obtain OHLC (Open, High, Low, Close) type historical and real-time data, as well as real-time tick trading data.
4. Data sampling. The ability to resample short-term data into long-term data, such as combining daily data into weekly data, tick data into second data, etc.
5. Generality requirements. The ability to send data to third-party libraries such as ta-lib for indicator calculation.
6. Multi-threading requirements. The ability to support multi-threaded indicator calculation, with data sourced from a single-threaded record.
7. Ease of use requirements. The ability to quickly access recent data, and the last batch of recent data, through indexing.

## Implementation Layers

To fulfill the overall requirements, the data module needs to be divided into three layers:

1. The underlying database module. The underlying layer can use various methods such as mmap, files, compression, and databases to store the data and allow operations, including compression.
2. The middle glue layer. This layer can obtain data from various interfaces, such as IB Gateway, Yahoo Finance, and local databases. It can also send data to the upper layer, including in-memory data and temporary disk storage data.
3. The upper in-memory data layer. This layer stores all real-time data and can directly send it to libraries like ta-lib for indicator calculations.
