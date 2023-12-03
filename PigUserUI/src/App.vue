<template>
  <div>
    <PageLoading />
    <router-view></router-view>
  </div>
</template>

<script setup lang="ts">
import { defineAsyncComponent, shallowRef } from 'vue';

const PageLoading = shallowRef(defineAsyncComponent(() => import('./components/common/PageLoading.vue')));

</script>