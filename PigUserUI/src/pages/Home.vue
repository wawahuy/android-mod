<template>
  <v-app>
    <div ref="refBg" class="tw-fixed"></div>
    <Header></Header>
    <v-main>
      <Scroll></Scroll>
      <Slogan></Slogan>
      <section id="home-sec-test">
        <Slogan></Slogan>
      </section>
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
:deep(.v-application) {
  background-color: transparent;
}
</style>

<script setup lang="ts">
import { shallowRef, defineAsyncComponent, ref, onMounted, onUnmounted } from 'vue';
import { AnimationRoundHandler } from '@/three/ladding/handler';
import { toggleDebug } from '@/three/common';
const Header = shallowRef(defineAsyncComponent(() => import('@/components/ladding/Header.vue')));
const Scroll = shallowRef(defineAsyncComponent(() => import('@/components/ladding/Scroll.vue')));
const Slogan = shallowRef(defineAsyncComponent(() => import('@/components/ladding/Slogan.vue')));

const refBg = ref<HTMLDivElement>();
const refHandler = ref<AnimationRoundHandler>();

const onKeyDown = (e: KeyboardEvent) => {
  if (e.shiftKey && e.key.toLowerCase() === 'd') {
    toggleDebug();
  }
}

onMounted(() => {
  // init gsap
  const container = refBg.value as HTMLDivElement;
  const handler = new AnimationRoundHandler(container);
  setTimeout(() => {
    handler.initGsap("#home-sec-test");
  }, 500);
  refHandler.value = handler;

  // register event
  window.addEventListener('keydown', onKeyDown);
});

onUnmounted(() => {
  // unregister event
  window.removeEventListener('keydown', onKeyDown);

  // release three looper
  refHandler.value?.release();
})
</script>