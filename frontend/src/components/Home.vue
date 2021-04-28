<template>
  <div class="text-center mt-3 mb-5">
    <!-- <img alt="Vue logo" src="./assets/logo.png" /> -->
    <span style="font-size: 100px" class="text-secondary">
      {{ weightCurrent }}g
    </span>
    <br />
    <span style="font-size: 20px" class="text-secondary">Peso atual</span>
  </div>

  <div class="container">
    <div class="row justify-content-center">
      <div class="col-4">
        <form class="row">
          <div class="mb-3">
            <label class="form-label">Qtd. de gramas ideal</label>
            <input class="form-control" type="number" v-model="weightIdeal"/>
            <div class="form-text">Enquanto estiver sendo monitorado o recipiente estará com este peso</div>
          </div>

          <div class="mb-3">
            <label class="form-label">Momentos de monitoramento</label>

            <div class="input-group mb-2" v-for="(v, index) in verifications" :key="index">
              <span class="input-group-text">Início e Fim</span>
              <input type="time" aria-label="Início" class="form-control" v-model="v.start"/>
              <input type="time" aria-label="Fim" class="form-control" v-model="v.end"/>
              <button class="btn btn-danger" type="button" @click="actionVerificationDelete(index)">-</button>
            </div>

            <div>
              <button v-if="verifications.length < 5" class="btn btn-outline-success" type="button" @click="actionVerificationAdd()">+</button>
            </div>
          </div>

          <hr/>

          <div class="col-12">
            <button type="button" class="col-12 btn btn-success" @click="actionSave()">
              Salvar
            </button>
          </div>
        </form>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
import { defineComponent } from "@vue/runtime-core";
import mqtt from 'mqtt'

interface Verification {
  start: string | null
  end: string | null
}

const MQTT_KEYS = {
  weightCurrent: 'isaken/feeds/alimentador.pesoatual',
  weightIdeal: 'isaken/feeds/alimentador.pesoideal',
  verifications: 'isaken/feeds/alimentador.momentosverificacao'
}

const Home = defineComponent({
  data() {
    return {
      clientMqtt: null as mqtt.MqttClient | null,
      weightCurrent: 0,
      weightIdeal: 0,
      verifications: [] as Verification[],
    };
  },
  mounted() {
    const weightCurrent = window.localStorage.getItem(MQTT_KEYS.weightCurrent)
    if (weightCurrent) {
      this.weightCurrent = Number(weightCurrent)
    }
    const weightIdeal = window.localStorage.getItem(MQTT_KEYS.weightIdeal)
    if (weightIdeal) {
      this.weightIdeal = Number(weightIdeal)
    }
    const verifications = window.localStorage.getItem(MQTT_KEYS.verifications)
    if (verifications) {
      this.verifications = JSON.parse(verifications!)
    }

    this.clientMqtt = this.connectMqtt()
  },
  methods: {
    connectMqtt() {
      const client = mqtt.connect('mqtt://io.adafruit.com', {
        clientId: `Automatic Feeder ${Date.now().toString()}`,
        username: process.env.VUE_APP_MQTT_USERNAME,
        password: process.env.VUE_APP_MQTT_PASSWORD
      })

      const subscribeMqtt = {
        [MQTT_KEYS.weightCurrent]: (message: string) => {
          this.weightCurrent = Number(message)
          this.saveLocalStorage()
        }
      } as any

      client.on('connect', function () {
        console.log('mqtt connected')

        client.on('message', function (topic, message) {
          console.log(`mqtt message ${topic}`)
          const subscribe = subscribeMqtt[topic]
          if (subscribe) {
            subscribe(message.toString())
          }
        })

        client.subscribe(MQTT_KEYS.weightCurrent, function () {
        })
        client.subscribe(MQTT_KEYS.weightIdeal, function () {
        })
        client.subscribe(MQTT_KEYS.verifications, function () {
        })
      })

      return client
    },
    actionVerificationAdd() {
      this.verifications.push({ start: null, end: null })
    },
    actionVerificationDelete(index: number) {
      this.verifications.splice(index, 1)
    },
    listToString() {
        return this.verifications.reduce((value, { start, end }) => (
                                  value !== '' ? `${value}_${start}/${end}` : `${start}/${end}`
                                ), '')
    },
    saveLocalStorage() {
      window.localStorage.setItem(MQTT_KEYS.weightCurrent, String(this.weightCurrent))
      window.localStorage.setItem(MQTT_KEYS.weightIdeal, String(this.weightIdeal))
      window.localStorage.setItem(MQTT_KEYS.verifications, JSON.stringify(this.verifications))
    },
    actionSave () {
      this.verifications = this.verifications.filter(({ start, end }) => start && end)
      const verificationsText = this.listToString()
      this.clientMqtt?.publish(MQTT_KEYS.weightIdeal, String(this.weightIdeal))
      this.clientMqtt?.publish(MQTT_KEYS.verifications, verificationsText)

      this.saveLocalStorage()
    },
  }
})
export default Home
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
</style>
