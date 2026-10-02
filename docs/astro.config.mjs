import { defineConfig } from 'astro/config';
import starlight from '@astrojs/starlight';
import mdx from '@astrojs/mdx';
import sitemap from '@astrojs/sitemap';
import { pluginLineNumbers } from '@expressive-code/plugin-line-numbers';
import { starlightBasePath } from 'starlight-base-path';
import starlightLinksValidator from 'starlight-links-validator';
import starlightScrollToTop from 'starlight-scroll-to-top';
import starlightAnnouncement from 'starlight-announcement';
import starlightFullViewMode from 'starlight-fullview-mode';
import { starlightIconsPlugin, starlightIconsIntegration } from 'starlight-plugin-icons';
import UnoCSS from 'unocss/astro';
import mermaid from 'astro-mermaid';
import { remarkCppReference } from './src/plugins/remark-cppreference.mjs';

export default defineConfig({
	site: 'https://morgancaron.github.io',
	base: process.env.GITHUB_ACTIONS === 'true' ? '/CppUtils' : '/',
	markdown: {
		remarkPlugins: [remarkCppReference],
	},
	integrations: [
		starlight({
			plugins: [
				starlightBasePath(),
				starlightLinksValidator({
					errorOnFallbackPages: false,
				}),
				starlightScrollToTop({
					tooltipText: {
						en: 'Scroll to top',
						fr: 'Retour en haut',
					},
					showTooltip: true,
				}),
				starlightAnnouncement({
					displayMode: 'rotate',
					rotateInterval: 6000,
					announcements: [
						{
							id: 'doc-announcement',
							content: {
								fr: 'Bienvenue sur la nouvelle documentation de CppUtils !',
								en: 'Welcome to the new CppUtils documentation!',
							},
							variant: 'tip',
						},
						{
							id: 'doc-wip',
							content: {
								fr: 'Cette documentation est encore en cours de construction et s\'enrichit régulièrement.',
								en: 'This documentation is still under construction and is being regularly updated.',
							},
							variant: 'caution',
						},
					],
				}),
				starlightIconsPlugin({
					sidebar: false,
					codeblock: true,
					extractSafelist: true,
				}),
				starlightFullViewMode({
					overrideWarnEnabled: true,
				}),
			],
			title: 'CppUtils',
			defaultLocale: 'fr',
			locales: {
				en: {
					label: 'English',
					lang: 'en',
				},
				fr: {
					label: 'Français',
					lang: 'fr',
				},
			},
			favicon: '/favicon.svg',
			logo: {
				src: './src/assets/logo.svg',
			},
			expressiveCode: {
				plugins: [pluginLineNumbers()],
				defaultProps: {
					showLineNumbers: false,
				},
				styleOverrides: {
					borderRadius: '8px',
				},
			},
			customCss: ['./src/styles/custom.css'],
			components: {
				PageTitle: './src/components/PageTitle.astro',
				TableOfContents: 'starlight-fullview-mode/overrides/TableOfContents.astro',
				Sidebar: 'starlight-fullview-mode/overrides/Sidebar.astro',
				ThemeSelect: './src/components/ThemeSelect.astro',
				PageFrame: './src/components/PageFrame.astro',
			},
			head: [
				{
					tag: 'script',
					attrs: {
						async: true,
						src: 'https://www.googletagmanager.com/gtag/js?id=G-9EYYYD173T',
					},
				},
				{
					tag: 'script',
					content: `
window.dataLayer = window.dataLayer || [];
function gtag() { dataLayer.push(arguments); }
gtag('js', new Date());
gtag('config', 'G-9EYYYD173T', {
	'anonymize_ip': true,
	'allow_google_signals': false,
	'allow_ad_personalization_signals': false
});
`,
				},
			],
			social: [
				{ icon: 'github', label: 'GitHub', href: 'https://github.com/MorganCaron/CppUtils' },
				{ icon: 'discord', label: 'Discord', href: 'https://discord.gg/mxZvun4' },
			],
			sidebar: [
				{
					label: 'Guides',
					translations: {
						fr: 'Guides',
					},
					items: [
						{
							label: 'Getting started',
							translations: { fr: 'Prise en main' },
							slug: 'guides/getting-started',
						},
						{
							label: 'Build & configuration',
							translations: { fr: 'Compilation & configuration' },
							slug: 'guides/build-configuration',
						},
						{
							label: 'RAII synchronization (Lockers)',
							translations: { fr: 'Synchronisation RAII (Lockers)' },
							slug: 'guides/lockers',
						},
					],
				},
				{
					label: 'API reference',
					translations: {
						fr: 'Référence d’API',
					},
					items: [{ autogenerate: { directory: 'reference', collapsed: true } }],
				},
			],
		}),
		mdx(),
		mermaid({
			autoTheme: true,
		}),
		UnoCSS(),
		starlightIconsIntegration({ extractSafelist: true }),
		sitemap(),
	],
});
