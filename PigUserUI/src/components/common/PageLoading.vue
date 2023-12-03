<template>
  <div class="fixed z-[9999999] w-[100vw]" v-if="state.percent < 100">
    <v-progress-linear
      :model-value="state.percent"
      color="#1b3f7d"
      buffer-value="0"
      stream
    ></v-progress-linear>
  </div>
</template>

<style scoped>
</style>

<script lang="ts" setup>
import { reactive, ref, watch } from 'vue';
import { useAppStore } from '../../stores/app.store';

const appStore = useAppStore();
const pageLoadingTimer = ref<NodeJS.Timeout>();

const state = reactive({
  percent: 100,
})

watch(() => appStore.isPageLoading, (isPageLoading) => {
  handleOpenBar(isPageLoading);
})

const handleOpenBar = (isPageLoading: boolean) => {
  if (pageLoadingTimer.value) {
    clearTimeout(pageLoadingTimer.value);
  }

  if (!isPageLoading) {
    state.percent = 95;
    pageLoadingTimer.value = setTimeout(() => state.percent = 100, 250);
    return;
  }

  state.percent = 20;
  streamBar();
}

const streamBar = () => {
  state.percent++;
  if (state.percent <= 80) {
    pageLoadingTimer.value = setTimeout(() => streamBar(), state.percent * 4);
  }
}

</script>