import { defineCollection } from 'astro:content';
import { docsLoader, i18nLoader } from '@astrojs/starlight/loaders';
import { docsSchema, i18nSchema } from '@astrojs/starlight/schema';
import { z } from 'astro/zod';

export const collections = {
	docs: defineCollection({
		loader: docsLoader(),
		schema: docsSchema({
			extend: z.object({
				namespace: z.string().optional(),
				templateParams: z.string().optional(),
				moduleImport: z.string().optional(),
				badge: z.string().optional(),
				specializationOf: z
					.object({
						name: z.string(),
						url: z.string().optional(),
						templateParams: z.string().optional(),
					})
					.optional(),
			}),
		}),
	}),
	i18n: defineCollection({ loader: i18nLoader(), schema: i18nSchema() }),
};
