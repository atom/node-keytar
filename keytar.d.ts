// Definitions by: Milan Burda <https://github.com/miniak>, Brendan Forster <https://github.com/shiftkey>, Hari Juturu <https://github.com/juturu>
// Adapted from DefinitelyTyped: https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/types/keytar/index.d.ts

/**
 * Get the stored password for the service and account.
 *
 * @param service The string service name.
 * @param account The string account name.
 *
 * @returns A promise for the password string.
 */
export declare function getPassword(service: string, account: string): Promise<string | null>;

/**
 * Get the stored password for the service and account.
 *
 * @param service The string service name.
 * @param account The string account name.
 *
 * @returns The password string, or null if not found.
 */
export declare function getPasswordSync(service: string, account: string): string | null;

/**
 * Add the password for the service and account to the keychain.
 *
 * @param service The string service name.
 * @param account The string account name.
 * @param password The string password.
 *
 * @returns A promise for the set password completion.
 */
export declare function setPassword(service: string, account: string, password: string): Promise<void>;

/**
 * Add the password for the service and account to the keychain.
 *
 * @param service The string service name.
 * @param account The string account name.
 * @param password The string password.
 *
 * @returns Nothing.
 */
export declare function setPasswordSync(service: string, account: string, password: string): void;

/**
 * Delete the stored password for the service and account.
 *
 * @param service The string service name.
 * @param account The string account name.
 *
 * @returns A promise for the deletion status. True on success.
 */
export declare function deletePassword(service: string, account: string): Promise<boolean>;

/**
 * Delete the stored password for the service and account.
 *
 * @param service The string service name.
 * @param account The string account name.
 *
 * @returns The deletion status. True on success.
 */
export declare function deletePasswordSync(service: string, account: string): boolean;

/**
 * Find a password for the service in the keychain.
 *
 * @param service The string service name.
 *
 * @returns A promise for the password string.
 */
export declare function findPassword(service: string): Promise<string | null>;

/**
 * Find a password for the service in the keychain.
 *
 * @param service The string service name.
 *
 * @returns The password string, or null if not found.
 */
export declare function findPasswordSync(service: string): string | null;

/**
 * Find all accounts and passwords for `service` in the keychain.
 *
 * @param service The string service name.
 *
 * @returns A promise for the array of found credentials.
 */
export declare function findCredentials(service: string): Promise<Array<{ account: string, password: string}>>;

/**
 * Find all accounts and passwords for `service` in the keychain.
 *
 * @param service The string service name.
 *
 * @returns The array of found credentials.
 */
export declare function findCredentialsSync(service: string): Array<{ account: string, password: string}>;
