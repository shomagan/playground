// Decode uplink function.
//
// Input is an object with the following fields:
// - bytes = Byte array containing the uplink payload, e.g. [255, 230, 255, 0]
// - fPort = Uplink fPort.
// - variables = Object containing the configured device variables.
//
// Output must be an object with the following fields:
// - data = Object representing the decoded payload.
function decodeUplink(input) {
    const bytes = input['bytes']
    const fport = parseInt(input['fPort'])

    const packetID = bytes[0];
    var decoded = {
        valid: true,
        err: 0,
        messages: []
    };


    if (packetID == 1) {
        const batt = bytes[1];
        const swv = bytes[2] + "." + bytes[3];
        const hwv = bytes[4] + "." + bytes[5];
        const wm = getWorkMode(bytes[6]);
        const ps = getPosStrategy(bytes[7]);
        const hiv = bytes[8] << 8 | bytes[9];
        const uiv = bytes[10] << 8 | bytes[11];
        const eiv = bytes[12] << 8 | bytes[13];
        const so = bytes[14];
        const sm = bytes[15];
        const me = Boolean(bytes[16]);
        const mt = bytes[17] << 8 | bytes[18];
        const ms = bytes[19] << 8 | bytes[20];
        const ml = Boolean(bytes[21]);
        const mo = bytes[22] << 8 | bytes[23];
        const se = Boolean(bytes[24]);
        const st = bytes[25] << 8 | bytes[26];
        const te = Boolean(bytes[27]);
        const tu = bytes[28] << 8 | bytes[29];
        const ts = bytes[30] << 8 | bytes[31];
        const th = (bytes[32] << 8 | bytes[33]) / 10;
        const tl = (bytes[34] << 8 | bytes[35]) / 10;
        const tr = bytes[36];
        const le = Boolean(bytes[37]);
        const lu = bytes[38] << 8 | bytes[39];
        const ls = bytes[40] << 8 | bytes[41];
        const lh = bytes[42] << 8 | bytes[43];
        const ll = bytes[44] << 8 | bytes[45];
        const lw = bytes[46];

        var sens = "unkown"
        var sosm = "unkown"


        if (so) {
            sens = "Temp-Light Sen. on"
        } else if (sm === 0) {
            sens = "Temp-Light Sen. off"
        }

        if (sm) {
            sosm = "Continuous Mode"
        } else if (sm === 0) {
            sosm = "Single Mode"
        }

        decoded = {
            packet: "Heartbeat",
            battery: batt,
            softwareV: swv,
            hardwareV: hwv,
            workMode: wm,
            posStrategy: ps,
            heartbeatInterval: hiv,
            uplinkInterval: uiv,
            EventInterval: uiv,
            sensors: sens,
            sosMode: sosm,
            motionEventMode: me,
            motionThreshold: mt,
            motionStartInterval: ms,
            motionlessEvent: ml,
            motionlessTimeout: mo,
            shockEvent: se,
            shockThreshold: st,
            tempEvent: te,
            tempInterval: tu,
            tempSample: ts,
            tempThresholdMax: th,
            tempThresholdMin: tl,
            tempThresholdRule: tr,
            lightEvent: le,
            lightInterval: lu,
            lightSample: ls,
            lightThresholdMax: lh,
            lightThresholdMin: ll,
            lightWarningType: lw
        }
    } else if (packetID == 2) {
        const batt = bytes[1];
        const swv = bytes[2] + "." + bytes[3];
        const hwv = bytes[4] + "." + bytes[5];
        const wm = getWorkMode(bytes[6]);
        const ps = getPosStrategy(bytes[7]);
        const hiv = bytes[8] << 8 | bytes[9];
        const uiv = bytes[10] << 8 | bytes[11];
        const eiv = bytes[12] << 8 | bytes[13];
        const so = bytes[14];
        const sm = bytes[15];
        var sens = "unkown"
        var sosm = "unkown"


        if (so) {
            sens = "Temp-Light Sen. on"
        } else if (sm === 0) {
            sens = "Temp-Light Sen. off"
        }

        if (sm) {
            sosm = "Continuous Mode"
        } else if (sm === 0) {
            sosm = "Single Mode"
        }

        decoded = {
            packet: "Heartbeat",
            battery: batt,
            softwareV: swv,
            hardwareV: hwv,
            workMode: wm,
            posStrategy: ps,
            heartbeatInterval: hiv,
            uplinkInterval: uiv,
            EventInterval: uiv,
            sensors: sens,
            sosMode: sosm
        }
    } else if (packetID == 5) {
        const batt = bytes[1];
        const wm = getWorkMode(bytes[2]);
        const ps = getPosStrategy(bytes[3]);
        const sm = bytes[4];
        var sosm = "unkown"

        if (sm) {
            sosm = "Continuous Mode"
        } else if (sm === 0) {
            sosm = "Single Mode"
        }

        decoded = {
            packet: "Heartbeat",
            battery: batt,
            workMode: wm,
            posStrategy: ps,
            sosMode: sosm
        }
    } else if (packetID == 6) {
        const sta = (bytes[1] << 16 | bytes[2] << 8 | bytes[3]);
        const mcnt = bytes[4];
        const utc = (bytes[5] << 24 | bytes[6] << 16 | bytes[7] << 8 | bytes[8]);
        const lon = (bytes[9] << 24 | bytes[10] << 16 | bytes[11] << 8 | bytes[12]) / 1000000;
        const lat = (bytes[13] << 24 | bytes[14] << 16 | bytes[15] << 8 | bytes[16]) / 1000000;
        const tmp = ((bytes[17] & 0x80 ? 0xFFFF << 16 : 0) | bytes[17] << 8 | bytes[18]) / 10;
        const lgt = (bytes[19] << 8 | bytes[20]);
        const batt = bytes[21];

        decoded = {
            packet: "GNSS Location & Sensor Data",
            eventStatus: sta,
            motionCount: mcnt,
            utcTime: utc,
            longitude: lon,
            latitude: lat,
            temperature: tmp,
            light: lgt,
            battery: batt
        }
    }else if (packetID == 7) {
        const utc = unixToDateTime(bytes[8] << 24 | bytes[9] << 16 | bytes[10] << 8 | bytes[11]);
        decoded = {
            packet: "hz 7",
            utcTime: utc
        }
    } else if (packetID == 9) {
        const sta = (bytes[1] << 16 | bytes[2] << 8 | bytes[3]);
        const mcnt = bytes[4];
        const utc = (bytes[5] << 24 | bytes[6] << 16 | bytes[7] << 8 | bytes[8]);
        const lon = (bytes[9] << 24 | bytes[10] << 16 | bytes[11] << 8 | bytes[12]) / 1000000;
        const lat = (bytes[13] << 24 | bytes[14] << 16 | bytes[15] << 8 | bytes[16]) / 1000000;
        const batt = bytes[17];

        decoded = {
            packet: "GNSS Location",
            eventStatus: sta,
            motionCount: mcnt,
            utcTime: utc,
            longitude: lon,
            latitude: lat,
            battery: batt
        }
    } else if (packetID == 0x11) {

        const utc = unixToDateTime(bytes[5] << 24 | bytes[6] << 16 | bytes[7] << 8 | bytes[8]);
        const batt = bytes[13];

        if (bytes[11] != 0x80 && bytes[9] != 0x80) {
            const tmp = ((bytes[9] & 0x80 ? 0xFFFF << 16 : 0) | bytes[9] << 8 | bytes[10]) / 10;
            const lgt = (bytes[11] << 8 | bytes[12]);

            decoded = {
                packet: "Positing status",
                utcTime: utc,
                temperature: tmp,
                light: lgt,
                battery: batt
            }
        } else {
            decoded = {
                packet: "Positing status",
                utcTime: utc,
                battery: batt
            }
        }



    } else if (packetID == 0x0D) {
        const ec = (bytes[1] << 24 | bytes[2] << 16 | bytes[3] << 8 | bytes[4]);

        decoded = {
            packet: "Positioning Timeout",
            longitude: 0,
            latitude: 0,
            errorCode: ec
        }

    } else {
        decoded = {
            packet: "unkown",
            packetID: packetID
        }
    }

    return {
        data: decoded
    }

}


function convertToDec(b) {
    var return_value = parseInt(b, 16);
    return return_value;
}

function unixToDateTime(unixtime) {
    var date = new Date(unixtime * 1000);
    var months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'];
    var hours = date.getHours();
    var minutes = "0" + date.getMinutes();
    var seconds = "0" + date.getSeconds();
    var year = date.getFullYear();
    var month = months[date.getMonth()];
    var day = date.getDate();
    var DateTime = day + '-' + month + '-' + year + ' ' + hours + ':' + minutes.substr(-2) + ':' + seconds.substr(-2);
    return DateTime;
}

function getPosStrategy(ps) {
    var psText = "";
    switch (ps) {
        case 0:
            psText = "Only GNSS";
            break;
        case 1:
            psText = "Only WiFi";
            break;
        case 2:
            psText = "WiFi + GNSS";
            break;
        case 3:
            psText = "GNSS + WiFi";
            break;
        case 4:
            psText = "Only Bluetooth";
            break;
        case 5:
            psText = "Bluetooth + WiFi";
            break;
        case 6:
            psText = "Bluetooth + GNSS";
            break;
        case 7:
            psText = "Bluetooth + WiFi + GNSS";
            break;
        default:
            psText = "unkown";
    }
    return psText;
}


function getWorkMode(wm) {
    var wmText = "";
    switch (wm) {
        case 0:
            wmText = "Standby Mode";
            break;
        case 1:
            wmText = "Periodic Mode";
            break;
        case 2:
            wmText = "Event Mode";
            break;
        default:
            wmText = "unkown";
    }
    return wmText;
}
// Encode downlink function.
//
// Input is an object with the following fields:
// - data = Object representing the payload that must be encoded.
// - variables = Object containing the configured device variables.
//
// Output must be an object with the following fields:
// - bytes = Byte array containing the downlink payload.
function encodeDownlink(input) {
  return {
    bytes: [225, 230, 255, 0]
  };
}
