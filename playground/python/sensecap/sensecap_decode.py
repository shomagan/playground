import datetime
import credentials
import sys
import re
import logging
__description__ = 'sensecap message decoding'


def unixToDateTime(unixtime):
   dt = datetime.datetime.fromtimestamp(unixtime)
   return str(dt)


def getPosStrategy(ps):
   psText = "unkown"
   if ps == 0:
      psText = "Only GNSS"
   elif ps == 1:
      psText = "Only WiFi"
   elif ps == 2:
      psText = "WiFi + GNSS"
   elif ps == 3:
      psText = "GNSS + WiFi"
   elif ps == 4:
      psText = "Only Bluetooth"
   elif ps == 5:
      psText = "Bluetooth + WiFi"
   elif ps == 6:
      psText = "Bluetooth + GNSS"
   elif ps == 7:
      psText = "Bluetooth + WiFi + GNSS"
   else:
      psText = "unkown"
   return psText


def getWorkMode(wm):
   wmText = "unkown"
   if wm == 0:
      wmText = "Standby Mode"
   elif wm == 1:
      wmText = "Periodic Mode"
   elif wm == 2:
      wmText = "Event Mode"
   else:
      wmText = "unkown"
   return wmText


def decoder(payload):
   packet_id = payload[0]
   decoded = {}
   if packet_id==1:
      batt = payload[1]
      swv = str(payload[2]) + "." + str(payload[3])
      hwv = str(payload[4]) + "." + str(payload[5])
      wm = getWorkMode(payload[6])
      ps = getPosStrategy(payload[7])
      hiv = payload[8] << 8 | payload[9]
      uiv = payload[10] << 8 | payload[11]
      eiv = payload[12] << 8 | payload[13]
      so = payload[14]
      sm = payload[15]
      me = bool(payload[16])
      mt = payload[17] << 8 | payload[18]
      ms = payload[19] << 8 | payload[20]
      ml = bool(payload[21])
      mo = payload[22] << 8 | payload[23]
      se = bool(payload[24])
      st = payload[25] << 8 | payload[26]
      te = bool(payload[27])
      tu = payload[28] << 8 | payload[29]
      ts = payload[30] << 8 | payload[31]
      th = (payload[32] << 8 | payload[33]) / 10
      tl = (payload[34] << 8 | payload[35]) / 10
      tr = payload[36]
      le = bool(payload[37])
      lu = payload[38] << 8 | payload[39]
      ls = payload[40] << 8 | payload[41]
      lh = payload[42] << 8 | payload[43]
      ll = payload[44] << 8 | payload[45]
      lw = payload[46]
      sens = "unkown"
      sosm = "unkown"
      if so:
         sens = "Temp-Light Sen. on"
      else:
         sens = "Temp-Light Sen. off"
      if sm:
         sosm = "Continuous Mode"
      else:
         sosm = "Single Mode"
      decoded = {
         "packet": "Heartbeat",
         "battery": batt,
         "softwareV": swv,
         "hardwareV": hwv,
         "workMode": wm,
         "posStrategy": ps,
         "heartbeatInterval": hiv,
         "uplinkInterval": uiv,
         "EventInterval": uiv,
         "sensors": sens,
         "sosMode": sosm,
         "motionEventMode": me,
         "motionThreshold": mt,
         "motionStartInterval": ms,
         "motionlessEvent": ml,
         "motionlessTimeout": mo,
         "shockEvent": se,
         "shockThreshold": st,
         "tempEvent": te,
         "tempInterval": tu,
         "tempSample": ts,
         "tempThresholdMax": th,
         "tempThresholdMin": tl,
         "tempThresholdRule": tr,
         "lightEvent": le,
         "lightInterval": lu,
         "lightSample": ls,
         "lightThresholdMax": lh,
         "lightThresholdMin": ll,
         "lightWarningType": lw
      }
   elif packet_id == 2:
      batt = payload[1]
      swv = str(payload[2]) + "." + str(payload[3])
      hwv = str(payload[4]) + "." + str(payload[5])
      wm = getWorkMode(payload[6])
      ps = getPosStrategy(payload[7])
      hiv = payload[8] << 8 | payload[9]
      uiv = payload[10] << 8 | payload[11]
      eiv = payload[12] << 8 | payload[13]
      so = payload[14]
      sm = payload[15]
      sens = "unkown"
      sosm = "unkown"
      if so:
         sens = "Temp-Light Sen. on"
      else:
         sens = "Temp-Light Sen. off"
      if sm:
         sosm = "Continuous Mode"
      else:
         sosm = "Single Mode"
      decoded = {
         "packet": "Heartbeat",
         "battery": batt,
         "softwareV": swv,
         "hardwareV": hwv,
         "workMode": wm,
         "posStrategy": ps,
         "heartbeatInterval": hiv,
         "uplinkInterval": uiv,
         "EventInterval": uiv,
         "sensors": sens,
         "sosMode": sosm
      }
   elif packet_id == 5:
      batt = payload[1]
      wm = getWorkMode(payload[2])
      ps = getPosStrategy(payload[3])
      sm = payload[4]
      sosm = "unkown"
      if sm:
         sosm = "Continuous Mode"
      else:
         sosm = "Single Mode"
      
      decoded = {
         "packet": "Heartbeat",
         "battery": batt,
         "workMode": wm,
         "posStrategy": ps,
         "sosMode": sosm
      }
   elif packet_id == 6:
      sta = (payload[1] << 16 | payload[2] << 8 | payload[3])
      mcnt = payload[4]
      utc = unixToDateTime(payload[5] << 24 | payload[6] << 16 | payload[7] << 8 | payload[8])
      lon = (payload[9] << 24 | payload[10] << 16 | payload[11] << 8 | payload[12]) / 1000000
      lat = (payload[13] << 24 | payload[14] << 16 | payload[15] << 8 | payload[16]) / 1000000
      high_half = 0
      if payload[17] & 0x80:
         high_half = 0xFFFF << 16
      tmp = (high_half | payload[17] << 8 | payload[18]) / 10
      lgt = (payload[19] << 8 | payload[20])
      batt = payload[21]
      decoded = {
         "packet": "GNSS Location & Sensor Data",
         "eventStatus": sta,
         "motionCount": mcnt,
         "utcTime": utc,
         "longitude": lon,
         "latitude": lat,
         "temperature": tmp,
         "light": lgt,
         "battery": batt
      }
   elif packet_id == 9:
      sta = (payload[1] << 16 | payload[2] << 8 | payload[3])
      mcnt = payload[4]
      utc = datetime.datetime.fromtimestamp(payload[5] << 24 | payload[6] << 16 | payload[7] << 8 | payload[8])
      lon = (payload[9] << 24 | payload[10] << 16 | payload[11] << 8 | payload[12]) / 1000000
      lat = (payload[13] << 24 | payload[14] << 16 | payload[15] << 8 | payload[16]) / 1000000
      batt = payload[17]
      logging.info(f"time - {utc}, lon - {lon}, lat - {lat}")
      decoded = {
         "packet": "GNSS Location",
         "eventStatus": sta,
         "motionCount": mcnt,
         "utcTime": utc,
         "longitude": lon,
         "latitude": lat,
         "battery": batt
      }
   elif packet_id == 0x11:
      utc = unixToDateTime(payload[5] << 24 | payload[6] << 16 | payload[7] << 8 | payload[8])
      batt = payload[13]
      if payload[11] != 0x80 and payload[9] != 0x80:
         if payload[9] & 0x80:
            high_half = 0xFFFF << 16
         tmp = (high_half | payload[9] << 8 | payload[10]) / 10
         lgt = (payload[11] << 8 | payload[12])
         decoded = {
            "packet": "Positing status",
            "utcTime": utc,
            "temperature": tmp,
            "light": lgt,
            "battery": batt
         }
      else:
         decoded = {
            "packet": "Positing status",
            "utcTime": utc,
            "battery": batt
         }
      
   elif packet_id == 0x0D:
        ec = (payload[1] << 24 | payload[2] << 16 | payload[3] << 8 | payload[4])

        decoded = {
            "packet": "Positioning Timeout",
            "longitude": 0,
            "latitude": 0,
            "errorCode": ec
        }
   else:
      decoded = {
         "packetType": "unkown",
         "packetID": packet_id
      }
   return decoded


if __name__ == "__main__":
   logging.basicConfig(level="INFO")
   logging.info(__description__)

   decoder(bytes.fromhex("090000000065b960c101314f2602fb28b463"))
