<template>
  <div class="text-center mt-3 mb-5">
    <!-- <img alt="Vue logo" src="./assets/logo.png" /> -->
    <span style="font-size: 100px" class="text-secondary">345g</span>
    <br />
    <span style="font-size: 20px" class="text-secondary">Peso atual</span>
  </div>

  <div class="container">
    <div class="row justify-content-center">
      <div class="col-4">
        <form class="row">
          <div class="mb-3">
            <label class="form-label">Qtd. de gramas ideal</label>
            <input class="form-control" type="number" />
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

            <button class="btn btn-outline-success" type="button" @click="actionVerificationAdd()">+</button>
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

interface Verification {
  start: string | null
  end: string | null
}

const Home = defineComponent({
  data() {
    return {
      verifications: [] as Verification[],
    };
  },
  mounted() {
    this.verifications = [
      { start: "10:30", end: "12:30" },
      { start: "15:30", end: "16:30" },
      { start: "18:30", end: "20:30" },
    ]
  },
  methods: {
    actionVerificationAdd() {
      this.verifications.push({ start: null, end: null })
    },
    actionVerificationDelete(index: number) {
      this.verifications.splice(index, 1)
    },
    actionSave () {
      
    },
  }
})
export default Home
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
</style>
