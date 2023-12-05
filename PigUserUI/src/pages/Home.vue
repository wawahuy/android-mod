<template>
  <v-app>
    <Header></Header>
    <v-main>
      <Scroll></Scroll>
      <Slogan></Slogan>
      <Slogan></Slogan>
    </v-main>
  </v-app>
</template>

<style scoped>
:deep(.login-button) {
  border-radius: 50px;
  color: black;
  font-size: 14px;
  border: 2px solid black;
  .v-btn__underlay {
    background-color: transparent;
  }
  &:hover {
    background-color: transparent;
  }
}
</style>

<script setup lang="ts">
import { shallowRef, defineAsyncComponent } from 'vue';
const Header = shallowRef(defineAsyncComponent(() => import('../components/ladding/Header.vue')));
const Scroll = shallowRef(defineAsyncComponent(() => import('../components/ladding/Scroll.vue')));
const Slogan = shallowRef(defineAsyncComponent(() => import('../components/ladding/Slogan.vue')));
</script>