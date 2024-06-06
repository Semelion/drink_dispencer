import React from 'react';
import { createAssistant, createSmartappDebugger } from '@salutejs/client';
import Button from './components/Button'

import './App.css';
import client from './components/mqtt_service';
// import Paho from 'paho-mqtt';

const initializeAssistant = (getState /*: any*/, getRecoveryState) => {
  if (process.env.NODE_ENV === 'development') {
    return createSmartappDebugger({
      token: process.env.REACT_APP_TOKEN ?? '',
      initPhrase: `Запусти ${process.env.REACT_APP_SMARTAPP}`,
      getState,
      // getRecoveryState: getState,
      nativePanel: {
        defaultText: 'Приготовьте рюмки',
        screenshotMode: false,
        tabIndex: -1,
    },
    });
  } else {
  return createAssistant({ getState });
  }
};

export class App extends React.Component {
  constructor(props) {
    super(props);
    // console.log('constructor');

    this.state = {
      notes: [{ id: Math.random().toString(36).substring(7), title: 'тест' }],
    };

    this.assistant = initializeAssistant(() => this.getStateForAssistant());

    this.assistant.on('data', (event /*: any*/) => {
      // console.log(`assistant.on(data)`, event);
      if (event.type === 'character') {
        // console.log(`assistant.on(data): character: "${event?.character?.id}"`);
      } else if (event.type === 'insets') {
        // console.log(`assistant.on(data): insets`);
      } else {
        const { action } = event;
        this.dispatchAssistantAction(action);
      }
    });

    this.assistant.on('start', (event) => {
      let initialData = this.assistant.getInitialData();

      // console.log(`assistant.on(start)`, event, initialData);
    });

    this.assistant.on('command', (event) => {
      // console.log(`assistant.on(command)`, event);
    });

    this.assistant.on('error', (event) => {
      // console.log(`assistant.on(error)`, event);
    });

    this.assistant.on('tts', (event) => {
      // console.log(`assistant.on(tts)`, event);
    });
  }

  componentDidMount() {
    console.log('componentDidMount');
  }

  getStateForAssistant() {
    // console.log('getStateForAssistant: this.state:', this.state);
    const state = {
      item_selector: {
        items: this.state.notes.map(({ id, title }, index) => ({
          number: index + 1,
          id,
          title,
        })),
        ignored_words: [
          'налей по', 'налить по', 'налить'
        ],
      },
    };
    // console.('getStateForAssistant: state:', state);
    return state;
  }

  dispatchAssistantAction(action) {
    // console.log('dispatchAssistantAction', action);
    if (action) {
      return this.dispence(action);
    }
  }

  dispence(action) {
    // console.log('DISPENCE');
    // console.log(action.note);

    // const mess = new Paho.Message(action.note);
    // mess.destinationName = "semelion/value"
    // client.send(mess);
    client.publish("semelion/value", action.note)
    console.log(action.note);
  }


  _send_action_value(action_id, value) {
    const data = {
      action: {
        action_id: action_id,
        parameters: {
          // значение поля parameters может быть любым, но должно соответствовать серверной логике
          value: value, // см.файл src/sc/noteDone.sc смартаппа в Studio Code
        },
      },
    };
    const unsubscribe = this.assistant.sendData(data, (data) => {
      // функция, вызываемая, если на sendData() был отправлен ответ
      const { type, payload } = data;
      // console.log('sendData onData:', type, payload);
      unsubscribe();
    });
  }



  render() {
    // console.log('render');
    return (
      <>
       <div className="container"><h2>Введите объем воды (мл)</h2>
         <input type="number" id="waterVolume"
            min="0" max="150" step="1" required
            autoFocus />
        <Button />

      </div>
    </>
    );
  }
}
