<template>
  <div class="text-center mt-3" style="margin-bottom: 80px">
    <!-- <img alt="Vue logo" src="./assets/logo.png" /> -->
    <span style="font-size: 100px" class="text-secondary">345g</span>
    <br/>
    <span style="font-size: 20px" class="text-secondary">Peso atual</span>
  </div>
  <!-- <HelloWorld msg="Welcome to Your Vue.js + TypeScript App" /> -->

  <div class="container">
    <div class="row justify-content-center">
      <div class="col-4">
        <form>
          <div class="mb-3">
            <label class="form-label">Qtd. de gramas ideal</label>
            <input class="form-control" type="number" />
            <div class="form-text">O recipiente encherá até este peso</div>
          </div>
          <div class="mb-3">
            <label class="form-label">Qtd. de gramas minima</label>
            <input class="form-control" type="number" />
            <div class="form-text">O recipiente encherá quando o peso estiver neste valor</div>
          </div>
          <div class="mb-3">
            <label class="form-label">Verificar peso a cada (segundos)</label>
            <input class="form-control" type="number" />
            <div class="form-text">Ex.: 120s (2m)</div>
          </div>

          <div>
            <label class="form-label">Momentos de verificação</label>
            <div class="input-group mb-2">
              <span class="input-group-text">Início e Fim</span>
              <input type="time" aria-label="Início" class="form-control" />
              <input type="time" aria-label="Fim" class="form-control" />
            </div>
            <div class="input-group mb-2">
              <span class="input-group-text">Início e Fim</span>
              <input type="time" aria-label="Início" class="form-control" />
              <input type="time" aria-label="Fim" class="form-control" />
            </div>
            <div class="input-group mb-2">
              <span class="input-group-text">Início e Fim</span>
              <input type="time" aria-label="Início" class="form-control" />
              <input type="time" aria-label="Fim" class="form-control" />
            </div>
            <div class="input-group mb-2">
              <span class="input-group-text">Início e Fim</span>
              <input type="time" aria-label="Início" class="form-control" />
              <input type="time" aria-label="Fim" class="form-control" />
            </div>
          </div>
        </form>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";
import HelloWorld from "./components/HelloWorld.vue";

@Options({
  components: {
    HelloWorld,
  },
})
export default class App extends Vue {}
</script>

<style>
/* #app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
} */
</style>
