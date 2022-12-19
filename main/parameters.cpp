inline void loadParameters() {
    param.Init();
    param.load("Tread",&Tread);
    param.load("Tsendtemps",&Tsendtemps);
    param.load("Ton",&Ton);
    param.load("Toff",&Toff);
    param.load("DT_TxMqtt",&DT_TxMqtt);
    param.load("DT_ActPump",&DT_ActPump);
    param.load("SecFpPumpOn",&SecFpPumpOn);
    param.load("MinTankTempToUseForWaterHeathing",&MinTankTempToUseForWaterHeathing);
    param.load("ThTFp",&ThTFp);

    }
