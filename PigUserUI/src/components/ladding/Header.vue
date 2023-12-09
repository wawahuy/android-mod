<template>
  <v-app-bar :elevation="0" class="tw-border-b-2 tw-border-[#5f5f5f] tw-px-2" :height="state.height">
    <v-container>
      <v-row class="tw-gap-1 tw-relative" align="center">
        <Logo />
        <v-spacer></v-spacer>
        <template v-if="display.mdAndUp">
          <v-btn
            v-for="item of menuItems"
            class="text-none default-button"
            variant="plain"
            @mouseenter="onMouseEnter($event)"
            @mouseleave="onMouseLeave"
          >
            {{ item.name }}
          </v-btn>
          <ButtonLogin></ButtonLogin>
        </template>
        <v-app-bar-nav-icon v-else variant="text" @click.stop="state.drawerMenu = !state.drawerMenu"></v-app-bar-nav-icon>
      </v-row>
    </v-container>
    <div
      class="tw-absolute tw-bottom-0 tw-h-[3px] tw-w-16 tw-bg-white tw-transition-all"
      :style="{
        left: state.lineHoverX,
        opacity: state.lineHoverOpacity,
      }"
    ></div>
  </v-app-bar>
</template>

<style scoped>
:deep(.login-button) {
  border-radius: 50px;
  color: white;
  font-size: 14px;
  border: 2px solid white;
  .v-btn__underlay {
    background-color: transparent;
  }
  &:hover {
    background-color: transparent;
  }
}
</style>

<script setup lang="ts">
import { ref, reactive } from 'vue';
import { useDisplay } from 'vuetify';
import * as _ from 'lodash';
import ButtonLogin from './ButtonLogin.vue';
import Logo from '@/components/common/Logo.vue';
import { onMounted, onUnmounted } from 'vue';

const display = ref(useDisplay());

const state = reactive({
  drawerMenu: false,
  lineHoverX: '100vw',
  lineHoverOpacity: 0,
  height: 100,
})

const menuItems = [
  { name: "Danh sách game" },
  { name: "Giới thiệu" },
]

const onMouseLeave = _.debounce(() => {
  state.lineHoverOpacity = 0;
}, 100);

const onMouseEnter = (event: MouseEvent) => {
  onMouseLeave.cancel();
  const e = event.target as HTMLDivElement;
  const rect = e.getBoundingClientRect();
  state.lineHoverX = ((rect.left + rect.right) / 2 - 32) + 'px';
  state.lineHoverOpacity = 1;
}

const onWindowScroll = () => {
  state.height = window.scrollY > 100 ? 60 : 100;
}

onMounted(() => {
  window.addEventListener('scroll', onWindowScroll);
})

onUnmounted(() => {
  window.removeEventListener('scroll', onWindowScroll);
})

</script>