<template>
  <Home />
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";
import Home from "./components/Home.vue";

@Options({
  components: {
    Home,
  },
  mounted() {
    console.log("Automatic Feeder");
  },
})
export default class App extends Vue {}
</script>
