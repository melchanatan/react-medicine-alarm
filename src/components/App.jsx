import { initializeApp } from "firebase/app";
import { getFirestore, collection, setDoc, doc, deleteDoc, onSnapshot } from "firebase/firestore"
import firebaseConfig from "../config";

import React, { useState, useEffect } from "react";
import Clock from "./Clock";
import Alarm from "./Alarm";

function App() {
  const firebaseApp = initializeApp(firebaseConfig);
  const db = getFirestore(firebaseApp);

  const [alarmList, setAlarmlist] = useState([]);
  
  useEffect(() => {
    const unsubscribe = onSnapshot(collection(db,"alarms"), (snapshot) => {
      const alarms = [];
      snapshot.forEach((doc) => {
        alarms.push({ id:doc.id, minute: doc.data().minute, hour: doc.data().hour });
      });
      alarms.sort((a,b) => {
        return a.id-b.id;
      });
      setAlarmlist(alarms);
      //console.log("update");
    });
    return () => unsubscribe();
  }, [db]);

  async function updateList(minute, hour) {
    try{
      await setDoc(doc(db, "alarms" , '' + (hour*60+minute)),{
        hour: hour,
        minute: minute
      });
      //console.log("Document written");
    } catch (e) {
      console.error("Error adding document: ", e);
    }
  }

  async function deleteAlarm(id) {
    await deleteDoc(doc(db, "alarms" , id))
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
