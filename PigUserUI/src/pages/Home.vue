<template>
  <v-app>
    <v-app-bar :elevation="0" class="tw-border-b-2 tw-border-[#e7e7e7] tw-px-2">
      <v-container>
        <v-row class="tw-gap-1" align="center">
          <Logo />
          <v-spacer></v-spacer>
          <template v-if="display.mdAndUp">
            <v-btn
              v-for="item of menuItems"
              class="text-none default-button"
              variant="plain"
            >
              {{ item.name }}
            </v-btn>
            <v-btn
              size="small"
              class="text-none login-button"
              variant="tonal"
              to="/login"
            >
              Đăng nhập
            </v-btn>
          </template>
          <v-app-bar-nav-icon v-else variant="text" @click.stop="state.drawerMenu = !state.drawerMenu"></v-app-bar-nav-icon>
        </v-row>
      </v-container>
    </v-app-bar>
    <v-main>
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
import { shallowRef, defineAsyncComponent, ref, reactive } from 'vue';
import { useDisplay } from 'vuetify';

const Logo = shallowRef(defineAsyncComponent(() => import('../components/common/Logo.vue')));
const display = ref(useDisplay());

const state = reactive({
  drawerMenu: false,
})

const menuItems = [
  { name: "Danh sách game" },
  { name: "Giới thiệu" },
]
</script>