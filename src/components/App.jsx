import { initializeApp } from "firebase/app";
import { getDatabase, ref, set, onValue, remove } from "firebase/database";
import firebaseConfig from "../config";

import React, { useState, useEffect } from "react";
import Clock from "./Clock";
import Alarm from "./Alarm";

function App() {
  const app = initializeApp(firebaseConfig);
  const db = getDatabase(app);

  const [alarmList, setAlarmlist] = useState([]);
  
  useEffect(() => {
    onValue(ref(db, 'alarms'), (snapshot) => {
      const alarms = [];
      snapshot.forEach((doc) => {
        alarms.push({ id:doc.key, minute:doc.val().minute, hour:doc.val().hour});
      });
      alarms.sort((a,b) => {
        return a.id-b.id;
      });
      setAlarmlist(alarms);
    });
  }, [db]);

  async function updateList(minute, hour) {
    try{
      await set(ref(db, 'alarms/' + (hour*60+minute)), {
        hour: hour,
        minute: minute
      });
    } catch (e) {
      console.error("Error adding document: ", e);
    }
  }

  async function deleteAlarm(id) {
    remove(ref(db, 'alarms/' + id));
  }

  return (
    <div className="app">
      <Clock updateList={updateList} />
      <div className="alarm__container">
        { alarmList && alarmList.map( (item, index) =>
          <Alarm
            minute={item.minute}
            hour={item.hour}
            key={item.id}
            id={item.id}
            deleteAlarm={deleteAlarm}
          />
        )}
      </div>
    </div>
  );
}

export default App;
