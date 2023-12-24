import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import path from 'path'
// import { splitVendorChunkPlugin } from 'vite'

// https://vitejs.dev/config/
export default defineConfig(({ mode }) => {
  return {
    plugins: [
      vue(),
      // splitVendorChunkPlugin(),
    ],
    // build: {
    //   rollupOptions: {
    //     output: {
    //       manualChunks(id: string) {
    //         if (id.includes('three')) {
    //           return 'three';
    //         }
    //       },
    //     },
    //   },
    // },
    resolve: {
      alias: {
        '@': path.resolve(__dirname, 'src'),
      }
    },
    esbuild: {
      pure: mode === 'production' ? ['console.log'] : [],
    },
    define: {
      // 'import.meta.vitest': mode === 'production' ? 'undefined' : true,
    },
  }
});
