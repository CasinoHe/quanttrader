﻿/* Copyright (C) 2019 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef TWS_API_CLIENT_ECLIENTMSGSINK_H
#define TWS_API_CLIENT_ECLIENTMSGSINK_H

struct EClientMsgSink
{
    virtual void serverVersion(int version, const char *time) = 0;
    virtual ~EClientMsgSink() {}
};
#endif